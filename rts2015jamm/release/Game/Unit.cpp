#include "Unit.h"
#include "Game.h"
#include "Player.h"
#include "Sprite_Anime.h"
#include "Group.h"
#include "Map.h"
#include "Cam.h"
#include "AI_AStar.h"
#include "ChecksCollision.h"
#include "HUDEventsCallbacks.h"
#include "Move_Handlers/Ground_Move_Handler.h"
#include "Move_Handlers/Air_Move_Handler.h"
#include "Attack_Handlers/Ground_Attack_Handler.h"
#include "Scripts/Script.hpp"
#include "States/FSM_Idle.h"
#include "States/FSM_Attacker.h"
#include "States/FSM_Event.h"
#include "../ID_glibrary_Extension/Map_Drawable.h"
#include "../Env.h"
#include <iostream>

void	__Unit_CbClickDown(ID::Control* _u, ID::Event* e, void*)
{
	std::cout << "UCD\n";
	Env*	env;
	Unit*	u;

	u = (Unit*)_u;
	env = Env::GetInstance();
	Selector_CbClickDown(e, env->gameScreen);
	Order_CbClickDown(e, u);
}

void	__Unit_CbClickUp(ID::Control* _u, ID::Event* e, void*)
{
	std::cout << "UCU\n";
	Env*	env;
	Unit*	u;

	u = (Unit*)_u;
	env = Env::GetInstance();
	Selector_CbClickUp(e, env->gameScreen);
}

void	__Unit_CbMotion(ID::Control* _u, ID::Event* e, void*)
{
	Env*	env;
	Unit*	u;

	u = (Unit*)_u;
	env = Env::GetInstance();
	Selector_CbMotion(e, env->gameScreen);
	Order_CbMotion(e);
	MapScrolling_CbMotion(e, env->gameScreen);
}

Unit::Unit(Unit_Model* Model, Player* p) : _player(p),
					   _model(Model),
					   _token(0),
					   _targetFollow(NULL),
					   _hold(false),
					   _targetAttack(NULL),
					   _updateSaver(-1),
					   _lastRefreshCam(-1),
					   _lastTickSelection(-1),
					   _path(NULL),
					   _previewingScript(false),
					   _previewingBuilding(false),
					   _alive(true),
					   _references(0)
{
	this->SetName("Unit");
	std::vector<Script_Model*>*		scripts;
	std::vector<Script_Model*>::iterator	it;
	std::vector<Script_Model*>::iterator	end;
	Script*					script;
	int					i;

	if (p != NULL)
		p->AddUnit(this);
	this->_HP = Model->GetHPMax();
	this->_PM = Model->GetPMMax();
	std::cout << this->_model->GetName() << " is out" << std::endl;
	i = 0;
	while (i < MAX_GROUPS)
	{
		this->_groups[i] = NULL;
		++i;
	}
	this->_pos.x = 0;
	this->_pos.y = 0;
	this->_targetMove.x = -1;
	this->_targetMove.y = -1;
	this->_targetAttackXY.x = -1;
	this->_targetAttackXY.y = -1;

	this->_animation.SetSpriteAnime(Model->GetSpriteAnime());
	if (this->_animation.GetSpriteAnime() == NULL)
		throw 1100;

	this->__AllocateMoveHandler(this->_model->GetMoveHandler());
	this->__AllocateAttackHandler(this->_model->GetAttackHandler());
	this->__AllocateStateMachine(this->_model->GetStateMachineID());

	scripts = this->_model->GetScripts();
	it = scripts->begin();
	end = scripts->end();
	while (it != end)
	{
		script = new Script(*it, this);
		if (script == NULL)
			throw 1140;
		this->_scripts.push_back(script);
		if (script->GetModel()->GetTriggeredAtStart() == TAS_TRUE)
			script->Call(0, 0, NULL);
		++it;
	}

	this->SetCbClickDown(__Unit_CbClickDown, this);
	this->SetCbClickUp(__Unit_CbClickUp, this);
	this->SetCbMotion(__Unit_CbMotion, this);
}

Unit::~Unit()
{
	delete this->_moveHandler;
	delete this->_attackHandler;
	delete this->_stateMachine;

}

void	Unit::Update(void)
{
	Game*				gm;
	ID::vector<Script*>::iterator	it;
	ID::vector<Script*>::iterator	end;

	gm = Game::GetInstance();
	it = this->_runningScripts.begin();
	end = this->_runningScripts.end();
	while (it != end)
	{
		(*it)->Apply();
		++it;
	}
	this->_stateMachine->Update();
}

void	Unit::SetHP(uint32_t data)
{
	this->_HP = data;
}

void	Unit::SetPM(uint32_t data)
{
	this->_PM = data;
}

void	Unit::SetToken(uint32_t data)
{
	this->_token = data;
}

void	Unit::SetPosition(Position data)
{
	this->SetPosition(data.x, data.y);
}

void	Unit::SetPosition(double x, double y)
{
	Cam*		c;
	ID::Rect	camFOV;
	int		_x;
	int		_y;

	c = Cam::GetInstance();
	camFOV.x = c->GetX() - c->GetCameraWidthRayon();
	camFOV.y = c->GetY() - c->GetCameraHeightRayon();
	camFOV.w = camFOV.x + c->GetCameraWidth();
	camFOV.h = camFOV.y + c->GetCameraHeight();

	c->InvalidateCases(this->_pos.x, this->_pos.y, this->_model->GetSize());
	this->__DeleteFromMap(this->_pos.x * CASE_SIDE_PIXELS, this->_pos.y * CASE_SIDE_PIXELS);
	this->_pos.x = x;
	this->_pos.y = y;
	_x = x * CASE_SIDE_PIXELS;
	_y = y * CASE_SIDE_PIXELS;
/*	std::cout << "SetPos("
		<< _x << ";" << _y << ") C ("
		<< camFOV.x << ";" << camFOV.y << ";" << camFOV.w << ";" << camFOV.h << ")"
		<< "\n";*/
	if (camFOV.x <= _x && _x <= camFOV.w &&
	    camFOV.y <= _y && _y <= camFOV.h)
	{
		c->GetMapDrawable()->DelGround(this, this->GetModel()->GetGround());
		c->GetMapDrawable()->AddGround(this, this->GetModel()->GetGround());
		this->SetPos(_x - c->GetOffsetX(),
			     _y - c->GetOffsetY());
	}
	c->InvalidateCases(this->_pos.x, this->_pos.y, this->_model->GetSize());
	this->__AddToMap(_x, _y);

	if (this->GetPreviewingScript() == true)
	{
		Game::GetInstance()->playerFocusScript->UnitUpdatePreview();
	}
}

void	Unit::SetUpdateSaver(uint32_t ls)
{
	this->_updateSaver = ls;
}

void	Unit::SetPlayer(Player* data)
{
	if (this->_player == data)
		return ;
	this->__EraseFromPlayer();
	this->_player = data;
}

void	Unit::SetTickSelection(uint32_t data)
{
	this->_lastTickSelection = data;
}

void	Unit::SetTargetMove(double x, double y)
{
	this->_targetMove.x = x;
	this->_targetMove.y = y;
	if (x != -1 && y != -1)
	{
		FSM_Event	e;

		e.type = FSM_ORDER_MOVE;
		e.move.x = x;
		e.move.y = y;
		this->_stateMachine->ApplyEvent(&e);
	}
}

void	Unit::SetTargetFollow(Unit* u)
{
	if (u->IsAlive() != NULL)
		u->Ref();
	else
		u = NULL;
	this->_targetFollow = u;
	if (u != NULL)
	{
		FSM_Event	e;

		e.type = FSM_ORDER_FOLLOW;
		e.follow.unit = u;
		this->_stateMachine->ApplyEvent(&e);
	}
}

void	Unit::SetHold(void)
{
	this->_hold = true;
	FSM_Event	e;

	e.type = FSM_ORDER_HOLD;
	e.hold.hold = true;
	this->_stateMachine->ApplyEvent(&e);
}

void	Unit::SetTargetAttackUnit(Unit* u)
{
	if (u->IsAlive() != NULL)
		u->Ref();
	else
		u = NULL;
	this->_targetAttack = u;
	if (u != NULL)
	{
		FSM_Event	e;

		e.type = FSM_ORDER_ATTACK;
		e.attack.x = 0;
		e.attack.y = 0;
		e.attack.unit = u;
		this->_stateMachine->ApplyEvent(&e);
	}
}

void	Unit::SetTargetAttackPos(double x, double y)
{
	this->_targetAttackXY.x = x;
	this->_targetAttackXY.y = y;
	if (x != -1 && y != -1)
	{
		FSM_Event	e;

		e.type = FSM_ORDER_ATTACK;
		e.attack.x = x;
		e.attack.y = y;
		e.attack.unit = NULL;
		this->_stateMachine->ApplyEvent(&e);
	}
}

void	Unit::SetPreviewingScript(bool data)
{
	this->_previewingScript = data;
}

void	Unit::SetPreviewingBuilding(bool data)
{
	this->_previewingBuilding = data;
}

uint32_t	Unit::GetHP(void)
{
	return (this->_HP);
}

uint32_t	Unit::GetPM(void)
{
	return (this->_PM);
}

uint32_t	Unit::GetToken(void)
{
	return (this->_token);
}

Position	Unit::GetPosition(void)
{
	return (this->_pos);
}

void		Unit::GetPositionXY(double* x, double* y)
{
	if (x != NULL)
		*x = this->_pos.x;
	if (y != NULL)
		*y = this->_pos.y;
}

uint32_t	Unit::GetUpdateSaver(void)
{
	return this->_updateSaver;
}

Unit_Model*	Unit::GetModel(void)
{
	return this->_model;
}

Player*	Unit::GetPlayer(void)
{
	return (this->_player);
}

uint32_t	Unit::GetTickSelection(void)
{
	return this->_lastTickSelection;
}

Group**	Unit::GetGroups(void)
{
	return &(this->_groups[0]);
}

void	Unit::GetTargetMove(double* x, double* y)
{
	if (x != NULL)
		*x = this->_targetMove.x;
	if (y != NULL)
		*y = this->_targetMove.y;
}

Unit*	Unit::GetTargetFollow(void)
{
	if (this->_targetFollow->IsAlive() == NULL)
	{
		this->_targetFollow->UnRef();
		this->_targetFollow = NULL;
	}
	return this->_targetFollow;
}

bool	Unit::GetHold(void)
{
	return this->_hold;
}

Unit*	Unit::GetTargetAttackUnit(void)
{
	if (this->_targetAttack->IsAlive() == NULL)
	{
		this->_targetAttack->UnRef();
		this->_targetAttack = NULL;
	}
	return this->_targetAttack;
}

void	Unit::GetTargetAttackPos(double* x, double* y)
{
	if (x != NULL)
		*x = this->_targetAttackXY.x;
	if (y != NULL)
		*y = this->_targetAttackXY.y;
}

bool	Unit::GetPreviewingScript(void)
{
	return this->_previewingScript;
}

bool	Unit::GetPreviewingBuilding(void)
{
	return this->_previewingBuilding;
}

Unit*	Unit::GetTargetAttackAuto(void)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;
	Map*		m;
	Cam*		c;
	double		xPos;
	double		yPos;
	int16_t		xMin;
	int16_t		xMax;
	uint16_t	yMin;
	uint16_t	yMax;
	int16_t		deltaX;
	int16_t		x;
	int		i;
	ID::Circle	unit;
	ID::Rect	r;

	m = Map::GetInstance();
	c = Cam::GetInstance();

	this->_stateMachine->GetUnit()->GetPositionXY(&xPos, &yPos);
	unit.center.x = xPos * CASE_SIDE_PIXELS;// + c->GetOffsetX();
	unit.center.y = yPos * CASE_SIDE_PIXELS;// + c->GetOffsetY();
	unit.radius = this->_stateMachine->GetUnit()->GetModel()->GetSize();

	r.w = CASE_SIDE_PIXELS;
	r.h = CASE_SIDE_PIXELS;

	xMin = (int)(unit.center.x - unit.radius) >> 5;
	xMax = (int)(unit.center.x + unit.radius) >> 5;
	yMin = (int)(unit.center.y - unit.radius) >> 5;
	yMax = (int)(unit.center.y + unit.radius) >> 5;

	if (xMin < 0)
		xMin = 0;
	deltaX = m->w - xMax - xMin - 1;
	i = yMin * m->w + xMin;
	while (yMin <= yMax)
	{
		x = xMin;
		while (x <= xMax)
		{
			r.x = (x << 5) + (r.w >> 1);
			r.y = (yMin << 5) + (r.h >> 1);
			if (CheckCollision_RectCircle(&r, &unit) == true)
			{
				it = m->cases[i].units.begin();
				end = m->cases[i].units.end();
				while (it != end)
				{
					if (this->_stateMachine->GetUnit()->GetAttackHandler()->CanAttack(*it) == true)
						return *it;
					++it;
				}
			}
			++i;
			++x;
		}
		++yMin;
		i = i + deltaX;
	}
	return NULL;
}

bool	Unit::IsAlly(void)
{
	Game*	g;

	g = Game::GetInstance();
	if (g->GetMe() == this->GetPlayer())// ||
//	    g->GetMe()->GetTeam() == this->GetPlayer()->GetTeam())
		return true;
	return false;
}

ID::vector<Script*>*	Unit::GetScripts(void)
{
	return &(this->_scripts);
}

void	Unit::CallScript(Script* s)
{
	this->_runningScripts.push_back(s);
}

void	Unit::RemoveScript(Script* s)
{
	ID::vector<Script*>::iterator	it;
	ID::vector<Script*>::iterator	end;

	it = this->_runningScripts.begin();
	end = this->_runningScripts.end();
	while (it != end)
	{
		if (*it == s)
		{
			if (this->_runningScripts.size() > 1)
			{
				*it = this->_runningScripts.back();
			}
			this->_runningScripts.pop_back();
			return ;
		}
		++it;
	}
}

Move_Handler*	Unit::GetMoveHandler(void)
{
	return this->_moveHandler;
}

Attack_Handler*	Unit::GetAttackHandler(void)
{
	return this->_attackHandler;
}

Unit*	Unit::IsAlive(void)
{
	if (this->_alive == false)
		return NULL;
	return this;
}

void	Unit::Die(void)
{
	if (this->_alive == true)
	{
		this->__DeleteFromMap(this->_pos.x * CASE_SIDE_PIXELS,
				      this->_pos.y * CASE_SIDE_PIXELS);
		Cam::GetInstance()->GetMapDrawable()->DelGround(this, this->GetModel()->GetGround());
		this->__EraseFromPlayer();
		if (this->_lastTickSelection == Game::GetInstance()->lastSelection)
			this->GetPlayer()->DelUnitSelection(this);
		if (this->GetPreviewingScript() == true)
			Game::GetInstance()->playerFocusScript->CancelPreview();
		this->_alive = false;
	}
	if (this->_references == 0)
		delete this;
}

void	Unit::Ref(void)
{
	++this->_references;
}

void	Unit::UnRef(void)
{
	--this->_references;
	if (this->_references == 0 &&
	    this->_alive == false)
		this->Die();
	else if (this->_references < 0)
		throw 1500;
}

int	Unit::RefreshToSurface(ID::Surface* s, int x, int y)
{
	if (this->_animation.GetSpriteAnime() == NULL)
		return 0;
	ID::Rect*	size;

	size = &(this->_animation.
		 GetSpriteAnime()->
		 GetRects()[this->_animation.Animate()]);
	if (this->_animation.
		GetSpriteAnime()->
		GetSprite()->
		BlitSurface(size,
			    s,
			    x + this->_x - (size->w >> 1),
			    y + this->_y - (size->h >> 1)) == -1)
		return -1;
	this->SetInvalidate(false);
	return 0;
}

/*void	Unit::SetPos(int x, int y)
{
	this->_AddUpdateRect();
	this->_x = x;
	this->_y = y;
	this->_AddUpdateRect();
}*/

bool	Unit::_OnClickDown(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickDown.x >= this->_x + this->_offset_x - this->_model->GetSize() &&
	    event->clickDown.y >= this->_y + this->_offset_y - this->_model->GetSize() &&
	    event->clickDown.x < this->_x + this->_offset_x + this->_model->GetSize() &&
	    event->clickDown.y < this->_y + this->_offset_y + this->_model->GetSize())
		return true;
	return false;
}

bool	Unit::_OnClickUp(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickUp.x >= this->_x + this->_offset_x - this->_model->GetSize() &&
	    event->clickUp.y >= this->_y + this->_offset_y - this->_model->GetSize() &&
	    event->clickUp.x < this->_x + this->_offset_x + this->_model->GetSize() &&
	    event->clickUp.y < this->_y + this->_offset_y + this->_model->GetSize())
		return true;
	return false;
}

bool	Unit::_OnMotion(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->motion.x >= this->_x + this->_offset_x - this->_model->GetSize() &&
	    event->motion.y >= this->_y + this->_offset_y - this->_model->GetSize() &&
	    event->motion.x < this->_x + this->_offset_x + this->_model->GetSize() &&
	    event->motion.y < this->_y + this->_offset_y + this->_model->GetSize())
		return true;
	return false;
}

void	Unit::__AddToMap(int16_t x, int16_t y)
{
	Map*		map;
	int16_t		xC;
	int16_t		yC;
	uint16_t	wC;
	uint16_t	hC;
	int		i;
	ID::Circle	c;
	ID::Rect	r;

	map = Map::GetInstance();

	c.center.x = x;
	c.center.y = y;
	c.radius = this->_model->GetSize();

	r.w = CASE_SIDE_PIXELS;
	r.h = CASE_SIDE_PIXELS;

	wC = (x + this->_model->GetSize()) >> 5;
	hC = (y + this->_model->GetSize()) >> 5;
	x -= this->_model->GetSize();
	y -= this->_model->GetSize();
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	yC = y >> 5;
	x = x >> 5;
	while (yC <= hC)
	{
		xC = x;
		i = yC * map->w + xC;
		while (xC <= wC)
		{
			r.x = (xC << 5) + (r.w >> 1);
			r.y = (yC << 5) + (r.h >> 1);
			if (CheckCollision_RectCircle(&r, &c))
				map->cases[i].AddUnit(this);
			++i;
			++xC;
		}
		++yC;
	}
}

void	Unit::__DeleteFromMap(int16_t x, int16_t y)
{
	Map*		map;
	int16_t		xC;
	int16_t		yC;
	uint16_t	wC;
	uint16_t	hC;
	int		i;
	ID::Circle	c;
	ID::Rect	r;

	map = Map::GetInstance();

	c.center.x = x;
	c.center.y = y;
	c.radius = this->_model->GetSize();

	r.w = CASE_SIDE_PIXELS;
	r.h = CASE_SIDE_PIXELS;

	wC = (x + this->_model->GetSize()) >> 5;
	hC = (y + this->_model->GetSize()) >> 5;
	x -= this->_model->GetSize();
	y -= this->_model->GetSize();
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	yC = y >> 5;
	x = x >> 5;
	while (yC <= hC)
	{
		xC = x;
		i = yC * map->w + xC;
		while (xC <= wC)
		{
			r.x = (xC << 5) + (r.w >> 1);
			r.y = (yC << 5) + (r.h >> 1);
			if (CheckCollision_RectCircle(&r, &c))
				map->cases[i].DelUnit(this);
			++i;
			++xC;
		}
		++yC;
	}
}

void	Unit::__EraseFromPlayer(void)
{
	if (this->_player != NULL)
		this->_player->DelUnit(this->_token);
}

void	Unit::__AllocateMoveHandler(int ID)
{
	if (ID == MHDLER_IMMOBILE)
	{
		this->_moveHandler = new Ground_Move_Handler(this);
		if (this->_moveHandler == NULL)
			throw 1112;
	}
	else if (ID == MHDLER_GROUND)
	{
		this->_moveHandler = new Ground_Move_Handler(this);
		if (this->_moveHandler == NULL)
			throw 1114;
	}
	else if (ID == MHDLER_AIR)
	{
		this->_moveHandler = new Air_Move_Handler(this);
		if (this->_moveHandler == NULL)
			throw 1116;
	}

	else
		throw 1119;
}

void	Unit::__AllocateAttackHandler(int ID)
{
	if (ID == AHDLER_NOATTACK)
	{
		this->_attackHandler = new Ground_Attack_Handler(this);
		if (this->_attackHandler == NULL)
			throw 1120;
	}
	else if (ID == AHDLER_GROUND)
	{
		this->_attackHandler = new Ground_Attack_Handler(this);
		if (this->_attackHandler == NULL)
			throw 1122;
	}
	else if (ID == AHDLER_AIR)
	{
		this->_attackHandler = new Ground_Attack_Handler(this);
		if (this->_attackHandler == NULL)
			throw 1124;
	}
	else if (ID == AHDLER_BOTH)
	{
		this->_attackHandler = new Ground_Attack_Handler(this);
		if (this->_attackHandler == NULL)
			throw 1126;
	}
	else
		throw 1129;
}

void	Unit::__AllocateStateMachine(int ID)
{
	if (ID == FSM_IDLE)
	{
		this->_stateMachine = new FSM_Idle(this);
		if (this->_stateMachine == NULL)
			throw 1200;
	}
	else if (ID == FSM_ATTACKER)
	{
		this->_stateMachine = new FSM_Attacker(this);
		if (this->_stateMachine == NULL)
			throw 1210;
	}
	else
		throw 1299;
}

