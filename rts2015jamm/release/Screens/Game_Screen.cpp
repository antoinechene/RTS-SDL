#include "Game_Screen.h"
#include "Main_Screen.h"
#include "../Env.h"
#include "../Tools.h"
#include "../Constantes.h"
#include "../Menu/Main_Menu.h"
#include "../Game/Map.h"
#include "../Game/Game.h"
#include "../Game/Data_Access.h"
#include "../Game/Cam.h"
#include "../Game/HUDEventsCallbacks.h"
#include "../Game/Scripts/Script.hpp"
#include "../Network/Protocol_Manager.h"
#include "../Database/Database.h"
#include "../ID_glibrary_Extension/Map_Drawable.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#include "../Game/Unit.h"
#include "../Game/Player.h"
#include "../Game/Building.h"
#include "../Game/Unit_Model.h"
#include "../Game/Production_DB.h"
#include "../Database/Database.h"

#define HUD_BUTTON_SIZE 50
#define HUD_SELECTED_SIZE 50

void	__Game_Screen__HUDCbMotion(ID::Control*, ID::Event* e, void* _gs)
{
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	Selector_CbMotion(e, gs);
	MapScrolling_CbMotion(e, gs);
}

void	__Game_Screen__HUDCbClickDown(ID::Control*, ID::Event* e, void* _gs)
{
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	Selector_CbClickDown(e, gs);
}

void	__Game_Screen__HUDCbClickUp(ID::Control*, ID::Event* e, void* _gs)
{
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	Selector_CbClickUp(e, gs);
}

inline void	SetCam(ID::Drawable* d, int x, int y)
{
	Cam*	c;
	int	offsetX;
	int	offsetY;
	int16_t	posX;
	int16_t	posY;
	int	caseX;
	int	caseY;

	c = Cam::GetInstance();
	d->GetOffset(&offsetX, &offsetY);
	d->GetPos(&posX, &posY);
	caseX = c->GetMapWidth() * (x - offsetX - posX) / d->GetWidth();
	caseY = c->GetMapHeight() * (y - offsetY - posY) / d->GetHeight();
	c->SetCam(caseX, caseY);
}

void	__Game_Screen__minimapCbClickDown(ID::Control* mm, ID::Event* e, void* _gs)
{
	if (e->clickDown.button == SDL_BUTTON_LEFT)
	{
		std::cout << e->clickDown.button << "\n";
		Game_Screen*	gs;

		gs = (Game_Screen*)_gs;
		gs->movingCam = true;
		SetCam((ID::Drawable*)mm, e->clickDown.x, e->clickDown.y);
	}
}

void	__Game_Screen__minimapCbClickUp(ID::Control*, ID::Event* e, void* _gs)
{
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	if (e->clickUp.button == SDL_BUTTON_LEFT)
		gs->movingCam = false;
	Selector_CbClickUp(e, gs);
}

void	__Game_Screen__minimapCbMotion(ID::Control* mm, ID::Event* e, void* _gs)
{
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	if (e->motion.state == SDL_BUTTON_LEFT)
	{
		if (gs->movingCam == true)
			SetCam((ID::Drawable*)mm, e->motion.x, e->motion.y);
		Selector_CbMotion(e, gs);
		MapScrolling_CbMotion(e, gs);
	}
}

void	__Game_Screen_orderFocus(ID::Button_Label* newFocus)
{
	Env*	env;
	env = Env::GetInstance();

	if (env->gameScreen->orderFocus != NULL)
	{
		env->gameScreen->orderFocus->SetBorder((uint16_t)0);
		env->gameScreen->orderFocus->SetBorderColor(0xFF0000);
	}
	env->gameScreen->orderFocus = newFocus;
	env->gameScreen->orderFocus->SetBorder(1);
	env->gameScreen->orderFocus->SetBorderColor(0xFF0000);
}

void	__Game_Screen__moveCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "move\n";
	Env*	env;
	env = Env::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b1);
}

void	__Game_Screen__attackCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "attack\n";
	Env*	env;
	env = Env::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b2);
}

void	__Game_Screen__patrolCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "patrol\n";
	Env*	env;
	env = Env::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b3);
}

void	__Game_Screen__holdCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "hold\n";
	Env*				env;
	Game*				gm;
	Protocol_Manager*		pm;
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;
	uint8_t				p;

	env = Env::GetInstance();
	gm = Game::GetInstance();
	pm = Protocol_Manager::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b4);
	it = gm->GetMe()->GetSelection()->begin();
	end = gm->GetMe()->GetSelection()->end();
	p = gm->GetMe()->GetID();
	while (it != end)
	{
		pm->prot.AddCmdGameOrderHold((*it)->GetToken(), p);
		++it;
	}
}

void	__Game_Screen__assistCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "assist\n";
	Env*	env;
	env = Env::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b5);
}

void	__Game_Screen__stopCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "stop\n";
	Env*	env;
	env = Env::GetInstance();

	__Game_Screen_orderFocus(&env->gameScreen->b6);
}

void	__Game_Screen__EscBuildCbClickDown(ID::Control*, ID::Event*, void*);
void	PreviewBuild(ID::Control*, ID::Event*, void*);
void	OrderToBuild(ID::Control*, ID::Event*, void*);
void	CallScript(ID::Control*, ID::Event*, void*);

void	__Game_Screen__buildCbClickDown(ID::Control*, ID::Event*, void* _gs)
{
	std::cout << "build\n";
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;

	gs->ProdHUD(*(Game::GetInstance()->GetMe()->GetSelection()->begin()), PreviewBuild);
	gs->b9.GetLabel()->SetText((char*)"X");
	gs->b9.SetCbClickDown(__Game_Screen__EscBuildCbClickDown, gs);
}

void	__Game_Screen__skillsCbClickDown(ID::Control*, ID::Event*, void* _gs)
{
	std::cout << "script\n";
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;

	gs->ScriptsHUD(*(Game::GetInstance()->GetMe()->GetSelection()->begin()), CallScript);
	gs->b9.GetLabel()->SetText((char*)"X");
	gs->b9.SetCbClickDown(__Game_Screen__EscBuildCbClickDown, gs);
}

void	__Game_Screen__EscBuildCbClickDown(ID::Control*, ID::Event*, void* _gs)
{
	std::cout << "esc build\n";
	Game_Screen*	gs;
	Game*		g;

	g = Game::GetInstance();
	gs = (Game_Screen*)_gs;
	if ((*g->GetMe()->GetSelection()->begin())->GetModel()->GetType() != 3)
		gs->UnitHUD();
	else if ((*g->GetMe()->GetSelection()->begin())->GetModel()->GetType() == 3)
		gs->ProdHUD((Building*)(*(g->GetMe()->GetSelection()->begin())), OrderToBuild);
	else if (g->GetMe()->GetSelection()->begin() == g->GetMe()->GetSelection()->end())
		gs->VoidHUD();
}

ID::Color	selectorColor = ID::Color(0xFF, 0xB0, 0x0F, 0x7F);

Game_Screen::Game_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT),
				HUD(UI_WIDTH, (UI_HEIGHT / 5) + 20, UI_FLAGS,
				    UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK),
				population(DEFAULT_FONT_PATH, 20, (char*)"0"),
				result(DEFAULT_FONT_PATH, 50, (char*)" "),
				b1(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b2(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b3(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b4(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b5(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b6(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b7(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b8(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				b9(HUD_BUTTON_SIZE, HUD_BUTTON_SIZE,
					UI_FLAGS, UI_RMASK, UI_GMASK,
					UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
					10, (char*)" ", (ID::Color*)&(ID::COLOR_GOLD)),
				_minimap((UI_WIDTH / 2), (UI_HEIGHT / 2) -
					(((UI_HEIGHT / 5) / 32) / 2), UI_FLAGS,
					 UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK),
				minimap((UI_WIDTH / 5), (UI_HEIGHT / 5) -
					(((UI_HEIGHT / 5) / 32) / 2), UI_FLAGS,
					 UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK),
				selector(0, 0, true, &selectorColor),
				infos(0)
{
	Env*		env;
	Database*	db;
	ID::Label*	lbl;
	int		i;

	env = Env::GetInstance();
	db = Database::GetInstance();

	this->SetAbstractParent(true);
	this->SetName("Game_Screen");

	this->HUD.SetName("HUD");
	this->HUD.SetPos(0, env->height - (env->height / 5) - 20);
	this->HUD.SetBackground((ID::Color*)&(ID::COLOR_GREY));
	this->HUD.SetCbClickDown(__Game_Screen__HUDCbClickDown, this);
	this->HUD.SetCbClickUp(__Game_Screen__HUDCbClickUp, this);
	this->HUD.SetCbMotion(__Game_Screen__HUDCbMotion, this);
	this->AddChild(&(this->HUD));

	this->population.SetPos(UI_WIDTH / 5 + 15, 25);
	this->HUD.AddChild(&(this->population));

	this->result.SetPos(UI_WIDTH / 5 + 15, 25);
	this->result.SetColor(ID::COLOR_GOLD);
	this->HUD.AddChild(&(this->result));

	this->b1.SetName("b1");
	this->b1.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 3)),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 3 - 20));
	this->b1.SetBackground(999999);
	this->b1.SetLabelPos(ID::REL_POS_CENTER);
	this->AddChild(&(this->b1));

	this->b2.SetName("b2");
	this->b2.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 2)),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 3 - 20));
	this->b2.SetBackground(999999);
	this->AddChild(&(this->b2));

	this->b3.SetName("b3");
	this->b3.SetPos((env->width - ((HUD_BUTTON_SIZE + 5))),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 3 - 20));
	this->b3.SetBackground(999999);
	this->AddChild(&(this->b3));

	this->b4.SetName("b4");
	this->b4.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 3)),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 2 - 20));
	this->b4.SetBackground(999999);
	this->AddChild(&(this->b4));

	this->b5.SetName("b5");
	this->b5.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 2)),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 2 - 20));
	this->b5.SetBackground(999999);
	this->AddChild(&(this->b5));

	this->b6.SetName("b6");
	this->b6.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) )),
			   (env->height - (HUD_BUTTON_SIZE + 5) * 2 - 20));
	this->b6.SetBackground(999999);
	this->AddChild(&(this->b6));

	this->b7.SetName("b7");
	this->b7.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 3 )),
			   (env->height - (HUD_BUTTON_SIZE + 5) - 20));
	this->b7.SetBackground(999999);
	this->AddChild(&(this->b7));

	this->b8.SetName("b8");
	this->b8.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) * 2)),
			   (env->height - (HUD_BUTTON_SIZE + 5) - 20));
	this->b8.SetBackground(999999);
	this->AddChild(&(this->b8));

	this->b9.SetName("b9");
	this->b9.SetPos((env->width - ((HUD_BUTTON_SIZE + 5) )),
			   (env->height - (HUD_BUTTON_SIZE + 5) - 20));
	this->b9.SetBackground(999999);
	this->AddChild(&(this->b9));

	this->minimap.SetName("Minimap");
	this->minimap.SetPos(5, env->height - (env->height / 5) - 10);
	this->minimap.SetBackground(&(this->_minimap));
//	this->minimap.SetBackground((ID::Color*)&(ID::COLOR_WHITE));
	this->minimap.SetCbMotion(__Game_Screen__minimapCbMotion, this);
	this->minimap.SetCbClickDown(__Game_Screen__minimapCbClickDown, this);
	this->minimap.SetCbClickUp(__Game_Screen__minimapCbClickUp, this);
	this->AddChild(&(this->minimap));

	this->selector.SetName("Selector");
	this->selector.SetOnScreen(false);
	this->AddChild(&(this->selector));

	this->orderFocus = 0;

	i = 1;
	while (db->getResourceModel(i) != NULL)
	{
		lbl = new ID::Label(DEFAULT_FONT_PATH, 20, (char*)"0");
		if (lbl == NULL)
			throw 1200;
		lbl->SetPos(UI_WIDTH / 5 + 15, 30 + 20 * i);
		this->HUD.AddChild(lbl);
		this->resources.push_back(lbl);
		++i;
	}
}


Game_Screen::~Game_Screen()
{
	std::vector<ID::Label*>::iterator	it;
	std::vector<ID::Label*>::iterator	end;

	it = this->resources.begin();
	end = this->resources.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
}

void	Game_Screen::RefreshMinimap()
{
	Map*		map;
	int		x;
	int		y;
	int		i;
	int		case_w;
	int		case_h;

	map = Map::GetInstance();
	y = 0;
	case_w = (UI_WIDTH / 4) / map->w;
	case_h = (UI_HEIGHT / 5) - 10 / map->h;
	i = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			this->_minimap.SetPixel(x, y, &(map->cases[i].animation.GetSpriteAnime()->color_case));
			++x;
			++i;
		}
		++y;
	}
}

void	Game_Screen::ApplyEvent(ID::Event* e)
{
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
		{
			Game*	gm;

			gm = Game::GetInstance();
			if (gm->playerFocusScript != NULL)
			{
				gm->playerFocusScript->CancelPreview();
			}
			else if (gm->playerFocusProd != NULL)
			{
				Cam::GetInstance()->GetMapDrawable()->DelGround(gm->shadowProd, ON_BACKGROUND);
				delete gm->shadowProd;
				gm->playerFocusProd = NULL;
				gm->shadowProd = NULL;
			}
			else

			{
				Env*	env;
				env = Env::GetInstance();

				env->SwitchScreen(env->mainScreen);
				Game::GetInstance()->CleanAll();
			}
		}
	}
}

inline void	RefreshSelector(Game_Screen* gs)
{
	if (gs->refreshSelector == false)
		return ;
	Cam*	c;
	int16_t	newX;
	int16_t	newY;
	int16_t	newW;
	int16_t	newH;

	c = Cam::GetInstance();
	if (gs->nXSelector >= gs->xSelector)
	{
		newX = gs->xSelector;
		newW = gs->nXSelector - gs->xSelector;
	}
	else
	{
		newX = gs->nXSelector;
		newW = gs->xSelector - gs->nXSelector;
	}
	if (gs->nYSelector >= gs->ySelector)
	{
		newY = gs->ySelector;
		newH = gs->nYSelector - gs->ySelector;
	}
	else
	{
		newY = gs->nYSelector;
		newH = gs->ySelector - gs->nYSelector;
	}
//	std::cout << "SetPos(" << newX - c->GetOffsetX() << ", " << newY - c->GetOffsetY() << ")" << std::endl;
//	std::cout << "SetDim(" << newW << ", " << newH << ")" << std::endl;
	if (newX - c->GetOffsetX() >= UI_WIDTH ||
	    newY - c->GetOffsetY() >= UI_HEIGHT ||
	    newX - c->GetOffsetX() + newW >= UI_WIDTH ||
	    newY - c->GetOffsetY() + newH >= UI_HEIGHT)
	{
		gs->refreshSelector = false;
		return ;
	}
	gs->selector.SetPos(newX - c->GetOffsetX(), newY - c->GetOffsetY());
	gs->selector.SetDimension(newW, newH);
	gs->refreshSelector = false;
}

void	Game_Screen::UnitHUD()
{
	Unit*	u;

	u = *(Game::GetInstance()->GetMe()->GetSelection()->begin());
	this->b1.GetLabel()->SetText((char*)"MOVE");
	this->b1.SetCbClickDown(__Game_Screen__moveCbClickDown, NULL);

	this->b2.GetLabel()->SetText((char*)"ATK");
	this->b2.SetCbClickDown(__Game_Screen__attackCbClickDown, NULL);

	this->b3.GetLabel()->SetText((char*)"PTRL");
	this->b3.SetCbClickDown(__Game_Screen__patrolCbClickDown, NULL);

	this->b4.GetLabel()->SetText((char*)"HOLD");
	this->b4.SetCbClickDown(__Game_Screen__holdCbClickDown, NULL);

	this->b5.GetLabel()->SetText((char*)"AST");
	this->b5.SetCbClickDown(__Game_Screen__assistCbClickDown, NULL);

	this->b6.GetLabel()->SetText((char*)"STOP");
	this->b6.SetCbClickDown(__Game_Screen__stopCbClickDown, NULL);

	if (Database::GetInstance()->getProductionList(u->GetModel()->GetID()).size() > 0)
	{
		this->b7.GetLabel()->SetText((char*)"BUILD");
		this->b7.SetCbClickDown(__Game_Screen__buildCbClickDown, this);
	}
	else
	{
		this->b7.GetLabel()->SetText((char*)" ");
		this->b7.SetCbClickDown(NULL, NULL);
	}

	if (u->GetScripts()->size() > 0)
	{
		this->b8.GetLabel()->SetText((char*)"SKILLS");
		this->b8.SetCbClickDown(__Game_Screen__skillsCbClickDown, this);
	}
	else
	{
		this->b8.GetLabel()->SetText((char*)" ");
		this->b8.SetCbClickDown(NULL, NULL);
	}

	this->b9.GetLabel()->SetText((char*)" ");
	this->b9.SetCbClickDown(0, NULL);
}

void	Game_Screen::VoidHUD()
{
	this->b1.GetLabel()->SetText((char*)" ");
	this->b1.SetCbClickDown(0, NULL);

	this->b2.GetLabel()->SetText((char*)" ");
	this->b2.SetCbClickDown(0, NULL);

	this->b3.GetLabel()->SetText((char*)" ");
	this->b3.SetCbClickDown(0, NULL);

	this->b4.GetLabel()->SetText((char*)" ");
	this->b4.SetCbClickDown(0, NULL);

	this->b5.GetLabel()->SetText((char*)" ");
	this->b5.SetCbClickDown(0, NULL);

	this->b6.GetLabel()->SetText((char*)" ");
	this->b6.SetCbClickDown(0, NULL);

	this->b7.GetLabel()->SetText((char*)" ");
	this->b7.SetCbClickDown(0, NULL);

	this->b8.GetLabel()->SetText((char*)" ");
	this->b8.SetCbClickDown(0, NULL);

	this->b9.GetLabel()->SetText((char*)" ");
	this->b9.SetCbClickDown(0, NULL);
}

void	PreviewBuild(ID::Control*, ID::Event*, void* _data)
{
	Production_DB*	data;
	Game*		game;
	Data_Access*	da;
	Animation*	anim;

	data = (Production_DB*)_data;
	game = Game::GetInstance();
	da = Data_Access::GetInstance();

	if (game->playerFocusProd == NULL)
	{
		game->shadowProd = new Light_Drawable;
		if (game->shadowProd == NULL)
			return ;
		game->shadowProd->SetCbClickDown(__LD__CbClickDown, Env::GetInstance()->gameScreen);
		game->shadowProd->SetCbMotion(__LD__CbMotion, Env::GetInstance()->gameScreen);
		anim = new Animation(da->GetUnitPicture(data->prodID));
		if (anim == NULL)
		{
			delete game->shadowProd;
			return ;
		}
		game->shadowProd->SetPicture(anim);
		game->playerFocusProd = data;
		Cam::GetInstance()->GetMapDrawable()->AddGround(game->shadowProd, ON_BACKGROUND);
	}
}

void	CallScript(ID::Control*, ID::Event*, void* _data)
{
	Script*	data;
	Game*	game;

	data = (Script*)_data;
	game = Game::GetInstance();

	if (game->playerFocusProd != NULL)
		return ;
	std::cout << "Calling script " << data->GetModel()->GetName() << "\n";
	if (data->GetModel()->GetPreview() == true)
	{
		data->CallPreview();
	}
	else
	{
		data->Call(0, 0, NULL);
	}
}

void	OrderToBuild(ID::Control*, ID::Event*, void* _data)
{
	Production_DB*			data;
	Game*				game;
	std::list<Unit*>::iterator	it;

	data = (Production_DB*)_data;
	game = Game::GetInstance();

	it = game->GetMe()->GetSelection()->begin();
	((Building*)(*it))->AddToBuildList(data);
}

void	Game_Screen::ProdHUD(Unit* u, void (*callback)(ID::Control*, ID::Event*, void*))
{
	Database*				db;
	std::vector<Production_DB*>		prodList;
	std::vector<Production_DB*>::iterator	it;

	db = Database::GetInstance();
	prodList = db->getProductionList(u->GetModel()->GetID());
	it = prodList.begin();

	this->VoidHUD();

	if (it != prodList.end())
	{
		std::cout << (*it) << std::endl;
		this->b1.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b1.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b2.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b2.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b3.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b3.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b4.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b4.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b5.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b5.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b6.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b6.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b7.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b7.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != prodList.end())
	{
		this->b8.GetLabel()->SetText(db->getUnitModel((*it)->prodID)->GetName());
		this->b8.SetCbClickDown(callback, (void*)(*it));
	}
	this->b9.GetLabel()->SetText((char*)" ");
	this->b9.SetCbClickDown(NULL, NULL);
}

void	Game_Screen::ScriptsHUD(Unit* u, void (*callback)(ID::Control*, ID::Event*, void*))
{
	ID::vector<Script*>::iterator	it;
	ID::vector<Script*>::iterator	end;

	it = u->GetScripts()->begin();
	end = u->GetScripts()->end();

	this->VoidHUD();
	if (it != end)
	{
		this->b1.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b1.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b2.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b2.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b3.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b3.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b4.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b4.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b5.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b5.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b6.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b6.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b7.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b7.SetCbClickDown(callback, (void*)(*it));
		++it;
	}
	if (it != end)
	{
		this->b8.GetLabel()->SetText((*it)->GetModel()->GetName());
		this->b8.SetCbClickDown(callback, (void*)(*it));
	}
	this->b9.GetLabel()->SetText((char*)" ");
	this->b9.SetCbClickDown(NULL, NULL);
}

void	Game_Screen::Refresh()
{
	Cam*	c;

	c = Cam::GetInstance();
	c->Refresh();
	c->RefreshScrolling();
	RefreshSelector(this);
}

void	Game_Screen::Enter()
{
	std::cout << "Entering GameScreen\n";
	Cam*	c;
	Env*	e;

	c = Cam::GetInstance();
	e = Env::GetInstance();
//	e->window->AddChild(c->GetMapDrawable());
}

void	Game_Screen::Leave()
{
	std::cout << "Leaving GameScreen\n";
	Cam*	c;
	Env*	e;

	c = Cam::GetInstance();
	e = Env::GetInstance();
//	e->window->GetMainControl()->DelChild(c->GetMapDrawable());
}

int	Game_Screen::RefreshSelectedList()
{
//#TODO Modifier l'appel de cette fonction, ne l'appeller que quand on a reellemment besoin (Focus sur le bon batiment, ne le faire qu'une seule fois, etc...)
	Env*					env;
	Game*					game;
	std::list<Unit*>::iterator		selected_it;
	std::list<Unit*>::iterator		selected_end;
	std::list<HUD_Selected*>::iterator	selectedList_it;
	std::list<HUD_Selected*>::iterator	selectedList_end;
	HUD_Selected*				HUDselected;
	int					i;

	env = Env::GetInstance();
	game = Game::GetInstance();

//	pthread_mutex_lock(&(env->displayMutex));
	if (this->infos == 0)
	{
		this->infos = new HUD_DisplayInfo();
		if (this->infos == NULL)
		{
//			pthread_mutex_unlock(&(env->displayMutex));
			return -1;
		}
	}

	selectedList_it = this->SelectedList.begin();
	selectedList_end = this->SelectedList.end();
	while (selectedList_it != selectedList_end)
	{
		delete *selectedList_it;
		++selectedList_it;
	}
	this->SelectedList.clear();

	i = 0;
	selected_it = game->GetMe()->GetSelection()->begin();
	selected_end = game->GetMe()->GetSelection()->end();
	while (selected_it != selected_end)
	{
		HUDselected = new HUD_Selected(i, (*selected_it));
		if (HUDselected == NULL)
		{
//			pthread_mutex_unlock(&(env->displayMutex));
			return -1;
		}
		env->gameScreen->SelectedList.push_back(HUDselected);
		++selected_it;
		++i;
	}
	if (i == 0)
	{
//		pthread_mutex_unlock(&(env->displayMutex));
		return 0;
	}
	if ((*game->GetMe()->GetSelection()->begin())->GetModel()->GetType() != 3)
		this->UnitHUD();
	else if ((*game->GetMe()->GetSelection()->begin())->GetModel()->GetType() == 3)
		this->ProdHUD((Building*)(*(game->GetMe()->GetSelection()->begin())), OrderToBuild);
	else if (game->GetMe()->GetSelection()->begin() == game->GetMe()->GetSelection()->end())
		this->VoidHUD();

	if (i == 1)
	{
		this->infos->Update((*game->GetMe()->GetSelection()->begin()));
		this->infos->Show();
	}
	else
		this->infos->Hide();
//	pthread_mutex_unlock(&(env->displayMutex));
	return 0;
}

int	Game_Screen::RefreshProductionList(Building* unit)
{
	std::list<Prod_Stack*>::iterator	prodList_it;
	std::list<Prod_Stack*>::iterator	prodList_end;
	std::list<HUD_Production*>::iterator	prodListDisp_it;
	std::list<HUD_Production*>::iterator	prodListDisp_end;
	HUD_Production*				HUDProd;
	int					i;

	prodListDisp_it = this->ProductionList.begin();
	prodListDisp_end = this->ProductionList.end();
	while (prodListDisp_it != prodListDisp_end)
	{
		delete ((*prodListDisp_it));
		++prodListDisp_it;
	}
	this->ProductionList.clear();

	i = 0;
	prodList_it = unit->GetProdList()->begin();
	prodList_end = unit->GetProdList()->end();
	while (prodList_it != prodList_end)
	{
		HUDProd = new HUD_Production(i, (*prodList_it));
		if (HUDProd == NULL)
			return -1;
		HUDProd->UpdateNB((*prodList_it)->nb + 1);
		this->ProductionList.push_back(HUDProd);
		++prodList_it;
		++i;
	}
	return 0;
}

void	Game_Screen::DisplayResult(int id)
{
	std::cout << id << std::endl;
	if (id == 0)
		this->result.SetText((char*)"You are victorious!");
	else if (id == 1)
		this->result.SetText((char*)"The got defeated. Ah-ah!");
	else if (id == 2)
		this->result.SetText((char*)"Draw?... Really?");
	else
		this->result.SetText((char*)"What the fuck?");
	this->result.SetOnScreen(true);
}


HUD_Selected::HUD_Selected(int pos,
			   Unit* unit) : button(HUD_SELECTED_SIZE, HUD_SELECTED_SIZE, UI_FLAGS,
						UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK,
						DEFAULT_FONT_PATH,
						10, unit->GetModel()->GetName(), (ID::Color*)&(ID::COLOR_GOLD)),
					  HPred(HUD_SELECTED_SIZE, 5, UI_FLAGS,
						UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK),
					  HPgreen((unit->GetHP() / unit->GetModel()->GetHPMax()) * HUD_SELECTED_SIZE,
						  5, UI_FLAGS,
						  UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK)
{
	Env*	env;

	env = Env::GetInstance();

	this->LinkedUnit = unit;

	this->SetPos(350 + ((HUD_SELECTED_SIZE + 5) * pos), (env->height - (HUD_SELECTED_SIZE + 5) * 2));

//	this->move.SetCbClickDown(__Game_Screen__moveCbClickDown, NULL);
	this->button.SetName("button");
	this->button.SetBackground(999999);
	env->gameScreen->AddChild(&(this->button));

	this->HPred.SetName("HPred");
	this->HPred.SetBackground((ID::Color*)&(ID::COLOR_RED));
	env->gameScreen->AddChild(&(this->HPred));

	this->HPgreen.SetName("HPgreen");
	this->HPgreen.SetBackground((ID::Color*)&(ID::COLOR_GREEN));
	env->gameScreen->AddChild(&(this->HPgreen));
}

HUD_Selected::~HUD_Selected()
{
	Env*	env;

	env = Env::GetInstance();

	env->gameScreen->DelChild(&(this->button));
	env->gameScreen->DelChild(&(this->HPred));
	env->gameScreen->DelChild(&(this->HPgreen));
}

void	HUD_Selected::SetPos(int x, int y)
{
	this->button.SetPos(x, y);
	this->HPred.SetPos(x, y + HUD_SELECTED_SIZE);
	this->HPgreen.SetPos(x, y + HUD_SELECTED_SIZE);
}

void	HUD_Selected::Show()
{
	this->button.SetOnScreen(true);
	this->HPred.SetOnScreen(true);
	this->HPgreen.SetOnScreen(true);
}

void	HUD_Selected::Hide()
{
	this->button.SetOnScreen(false);
	this->HPred.SetOnScreen(false);
	this->HPgreen.SetOnScreen(false);
}

HUD_DisplayInfo::HUD_DisplayInfo() : name(DEFAULT_FONT_PATH, 20, (char*)" "),
				     HP(DEFAULT_FONT_PATH, 20, (char*)" "),
				     ATK(DEFAULT_FONT_PATH, 20, (char*)" "),
				     DEF(DEFAULT_FONT_PATH, 20, (char*)" "),
				     portrait(150, 150, UI_FLAGS,
					      UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK),
				     lastUnit(0)
{
	Env*		env;

	env = Env::GetInstance();

	this->name.SetName("name");
	this->name.SetPos((env->width / 2),
			   (env->height - 25 * 4) - 100);
	this->name.SetOnScreen(false);
	env->gameScreen->AddChild(&(this->name));

	this->HP.SetName("HP");
	this->HP.SetPos((env->width / 2),
			   (env->height - 25 * 3) - 100);
	this->HP.SetOnScreen(false);
	env->gameScreen->AddChild(&(this->HP));

	this->ATK.SetName("ATK");
	this->ATK.SetPos((env->width / 2),
			   (env->height - 25 * 2) - 100);
	this->ATK.SetOnScreen(false);
	env->gameScreen->AddChild(&(this->ATK));

	this->DEF.SetName("DEF");
	this->DEF.SetPos((env->width / 2),
			   (env->height - 25) - 100);
	this->DEF.SetOnScreen(false);
	env->gameScreen->AddChild(&(this->DEF));

//	this->portrait->SetPos((env->width / 2 - 200), (env->height - 200));
//	this->portait->BlitSurface(&(this->_animation.GetSpriteAnime()->GetRects()[this->]))
//	this->portrait->SetOnScreen(false);
//	env->gameScreen->AddChild(this->portrait);
}

void	HUD_DisplayInfo::Update(Unit* unit)
{
	Data_Access*	DA;
	Env*		env;
	char		hp[9];

	DA = Data_Access::GetInstance();
	env = Env::GetInstance();

	if (unit != this->lastUnit)
	{
		this->name.SetText(unit->GetModel()->GetName());
		this->ATK.SetText(id_itoa(unit->GetModel()->GetAtk()));
		this->DEF.SetText(id_itoa(unit->GetModel()->GetDef()));
	//	this->portrait = DA->GetPortraitPicture(unit->GetModel()->GetID())->GetSprite();
	//	this->portrait->SetInvalidate(true);
	}
	//	DA->GetPortraitPicture(unit->GetModel()->GetID())->BlitPict((env->width / 2 - 200), (env->height - 200), &(this->portrait));
	strcpy(hp, id_itoa(unit->GetHP()));
	strcat(hp, " / ");
	strcat(hp, id_itoa(unit->GetModel()->GetHPMax()));
	this->HP.SetText(hp);
}


void	HUD_DisplayInfo::Show()
{
	Env*					env;
	std::list<HUD_Selected*>::iterator	it;

	env = Env::GetInstance();
	it = env->gameScreen->SelectedList.begin();

	this->name.SetOnScreen(true);
	this->HP.SetOnScreen(true);
	this->ATK.SetOnScreen(true);
	this->DEF.SetOnScreen(true);
//	this->portrait->SetOnScreen(true);
	(*it)->Hide();
}

void	HUD_DisplayInfo::Hide()
{
	this->name.SetOnScreen(false);
	this->HP.SetOnScreen(false);
	this->ATK.SetOnScreen(false);
	this->DEF.SetOnScreen(false);
//	this->portrait->SetOnScreen(false);
}

char*	gimmeThisUnitName(int id)
{
	Database*	db;

	db = Database::GetInstance();

	return (db->getUnitModel(id)->GetName());
}

void	CancelBuildCallback(ID::Control* mm, ID::Event* e, void* _data)
{
	(void)mm;
	(void)e;
	Prod_Stack*	data;
	Env*		env;
	Game*		game;
	
	env = Env::GetInstance();
	game = Game::GetInstance();
	data = (Prod_Stack*)_data;
	if (data->nb > 0)
	{
		--data->nb;
		data->LinkedButton->UpdateNB(data->nb + 1);
	}
//#TODO Rendre les ressources depensees
	else
	{
		std::list<HUD_Production*>::iterator	it;
		Env*	env;

		env = Env::GetInstance();
		it = env->gameScreen->ProductionList.begin();
		while ((*it) != data->LinkedButton || it == env->gameScreen->ProductionList.end())
			++it;
		env->gameScreen->ProductionList.erase(it);
		delete	data;
	//	(Building*)(game->GetMe()->GetSelection()->front())->_buildingTime = 0;
		env->gameScreen->RefreshProductionList((Building*)game->GetMe()->GetSelection()->front());
	}
}


HUD_Production::HUD_Production(int pos, Prod_Stack* unit) : InProd(HUD_SELECTED_SIZE, HUD_SELECTED_SIZE,
								   UI_FLAGS, UI_RMASK, UI_GMASK,
								   UI_BMASK, UI_AMASK, DEFAULT_FONT_PATH,
								   10, gimmeThisUnitName(unit->prodDB->prodID),
									(ID::Color*)&(ID::COLOR_GOLD)),
								nb(DEFAULT_FONT_PATH, 10, (char*)"1",
								    (ID::Color*)&ID::COLOR_WHITE),
								prodTime(0, 0, true,
									(ID::Color*)&(ID::COLOR_GOLD))
//								prodTime(0, 5, UI_FLAGS,
//					 			     UI_RMASK, UI_GMASK, UI_BMASK, UI_AMASK)
{
	Env*	env;
	env = Env::GetInstance();

	this->SetPos(350 + ((HUD_SELECTED_SIZE + 5) * pos), (env->height - (HUD_SELECTED_SIZE + 5) * 2));

	this->InProd.SetName("InProd");
	this->InProd.SetBackground(999999);
	this->InProd.SetCbClickDown(CancelBuildCallback, unit);
	env->gameScreen->AddChild(&(this->InProd));

	env->gameScreen->AddChild(&(this->nb));

	this->prodTime.SetName("prodTime");
	this->prodTime.SetBackground((ID::Color*)&(ID::COLOR_BLUE));
	env->gameScreen->AddChild(&(this->prodTime));

	unit->LinkedButton = this;
	this->Show();
}

HUD_Production::~HUD_Production()
{
	Env*	env;

	env = Env::GetInstance();

	env->gameScreen->DelChild(&(this->InProd));
	env->gameScreen->DelChild(&(this->prodTime));
	env->gameScreen->DelChild(&(this->nb));
}

void	HUD_Production::SetPos(int x, int y)
{
	this->InProd.SetPos(x, y);
	this->nb.SetPos(x + 5, y);
	this->prodTime.SetPos(x, y + HUD_SELECTED_SIZE);
}

void	HUD_Production::Show()
{
	this->InProd.SetOnScreen(true);
	this->prodTime.SetOnScreen(true);
	this->nb.SetOnScreen(true);
}

void	HUD_Production::Hide()
{
	this->InProd.SetOnScreen(false);
	this->prodTime.SetOnScreen(false);
	this->nb.SetOnScreen(false);
}

void	HUD_Production::Update(Building* builder)
{
//#TODO Transformer ce Drawable en GRect
	this->prodTime.Resize(builder->GetProdPercent() / 2, 5);
	this->prodTime.SetInvalidate(true);
}

void	HUD_Production::UpdateNB(int nb)
{
	char*	tmp;

	tmp = id_itoa(nb);
	this->nb.SetText(tmp);
	delete tmp;
}
