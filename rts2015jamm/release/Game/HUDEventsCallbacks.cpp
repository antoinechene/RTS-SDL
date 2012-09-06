#include "HUDEventsCallbacks.h"
#include "ChecksCollision.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Cam.h"
#include "Unit.h"
#include "Move_Handlers/Move_Handler.h"
#include "AI_AStar.h"
#include "Scripts/Script.hpp"
#include "../Env.h"
#include "../Screens/Game_Screen.h"
#include "../Network/Protocol_Manager.h"
#include "../ID_glibrary_Extension/Map_Drawable.h"
#include <vector>
#include <sys/time.h>

void	Selector_CbClickDown(ID::Event* e, Game_Screen* gs)
{
	if (e->clickDown.button != ID::BUTTON_LEFT)
		return ;
	if (Game::GetInstance()->playerFocusScript != NULL ||
	    Game::GetInstance()->playerFocusProd != NULL)
		return ;
	Cam*	c;

	c = Cam::GetInstance();
	gs->selector.SetOnScreen(true);
	gs->selector.SetDimension(1, 1);
	gs->xSelector = c->GetOffsetX() + e->clickDown.x;
	gs->ySelector = c->GetOffsetY() + e->clickDown.y;
	gs->nXSelector = gs->xSelector;
	gs->nYSelector = gs->ySelector;
	gs->refreshSelector = true;
	gs->movingSelector = true;
}

void	Selector_CbClickUp(ID::Event* e, Game_Screen* gs)
{
	if (e->clickUp.button != ID::BUTTON_LEFT ||
	    gs->movingSelector == false)
		return ;
	if (Game::GetInstance()->playerFocusScript != NULL ||
	    Game::GetInstance()->playerFocusProd != NULL)
		return ;
	gs->selector.SetOnScreen(false);
	gs->movingSelector = false;
	GetSelection(&(gs->selector));
}

void	Selector_CbMotion(ID::Event* e, Game_Screen* gs)
{
	if (e->motion.state != ID::BUTTON_LEFT ||
	    gs->movingSelector == false)
		return ;
	if (Game::GetInstance()->playerFocusScript != NULL ||
	    Game::GetInstance()->playerFocusProd != NULL)
		return ;
	if (gs->selector.GetOnScreen() == true)
	{
		Cam*	c;

		c = Cam::GetInstance();
		if (e->motion.y > c->GetCameraHeight() - 20)
			e->motion.y = c->GetCameraHeight() - 20;
		gs->nXSelector = e->motion.x + c->GetOffsetX();
		gs->nYSelector = e->motion.y + c->GetOffsetY();
		gs->refreshSelector = true;
	}
}

void	Order_CbClickDown(ID::Event* e, Light_Drawable*)
{
	Cam*	c;
	Game*	gm;

	c = Cam::GetInstance();
	gm = Game::GetInstance();
	if (e->clickDown.button == ID::BUTTON_LEFT)
	{
		if (gm->playerFocusScript != NULL)
		{
			gm->playerFocusScript->ApplyEventPreview(e, NULL);
		}
		else if (gm->playerFocusProd != NULL)
		{
			c->GetMapDrawable()->DelGround(gm->shadowProd, ON_BACKGROUND);
			delete gm->shadowProd;
			gm->playerFocusProd = NULL;
			gm->shadowProd = NULL;
			std::cout << "Order to construct\n";
		}
	}
	else if (e->clickDown.button == ID::BUTTON_RIGHT)
	{
		std::cout << "Order_CbClickDown(LD)\n";
		Protocol_Manager*		pm;
		std::list<Unit*>::iterator	it;
		std::list<Unit*>::iterator	end;
		std::list<Unit*>*		selection;
		double				offsetCamX;
		double				offsetCamY;

		pm = Protocol_Manager::GetInstance();

		if (gm->playerFocusScript != NULL)
			gm->playerFocusScript->CancelPreview();
		else if (gm->playerFocusProd != NULL)
		{
			c->GetMapDrawable()->DelGround(gm->shadowProd, ON_BACKGROUND);
			delete gm->shadowProd;
			gm->playerFocusProd = NULL;
			gm->shadowProd = NULL;
		}

		offsetCamX = c->GetOffsetX() / CASE_SIDE_PIXELS;
		offsetCamY = c->GetOffsetY() / CASE_SIDE_PIXELS;
		selection = gm->players[gm->myID]->GetSelection();
		it = selection->begin();
		end = selection->end();
		while (it != end)
		{
			std::cout << "AddCmdGameOrderMove()" << std::endl;
			pm->prot.AddCmdGameOrderMove(offsetCamX + e->clickDown.x / CASE_SIDE_PIXELS,
						     offsetCamY + e->clickDown.y / CASE_SIDE_PIXELS,
						     (*it)->GetToken(),
						     gm->myID);
			++it;
		}
	}
}

void	Order_CbClickDown(ID::Event* e, Unit* u)
{
	std::cout << "Order_CbClickDown(UNIT)\n";
	Game*	gm;

	gm = Game::GetInstance();
	if (e->clickDown.button == ID::BUTTON_LEFT)
	{
		if (gm->playerFocusScript != NULL)
			gm->playerFocusScript->ApplyEventPreview(e, u);
	}
	else if (e->clickDown.button == ID::BUTTON_RIGHT)
	{
		if (gm->playerFocusScript != NULL)
		{
			gm->playerFocusScript->CancelPreview();
			return ;
		}
		Protocol_Manager*		pm;
		std::list<Unit*>::iterator	it;
		std::list<Unit*>::iterator	end;
		std::list<Unit*>*		selection;

		pm = Protocol_Manager::GetInstance();

		selection = gm->players[gm->myID]->GetSelection();
		it = selection->begin();
		end = selection->end();
		if (u->IsAlly() == true)
		{
			while (it != end)
			{
				std::cout << "AddCmdGameOrderFollow()" << std::endl;
				pm->prot.AddCmdGameOrderFollow(u->GetToken(),
							       u->GetPlayer()->GetID(),
							       (*it)->GetToken(),
							       gm->myID);
				++it;
			}
		}
		else
		{
			while (it != end)
			{
				std::cout << "AddCmdGameOrderAttack()" << std::endl;
				pm->prot.AddCmdGameOrderAttack(u->GetToken(),
							       u->GetPlayer()->GetID(),
							       (*it)->GetToken(),
							       gm->myID);
				++it;
			}
		}
	}
}

void	Order_CbMotion(ID::Event* e)
{
	Game*	gm;
	Cam*	c;

	gm = Game::GetInstance();
	c = Cam::GetInstance();
	if (gm->playerFocusScript != NULL)
	{
		gm->playerFocusScript->ApplyEventPreview(e, NULL);
	}
	else if (gm->playerFocusProd != NULL)
	{
		ID::Rect*	size;

		size = gm->shadowProd->GetPicture()->GetSpriteAnime()->GetRects();
		gm->shadowProd->SetPos(e->motion.x - (size->w >> 1),
				       e->motion.y - (size->h >> 1));
	}
}

void	MapScrolling_CbMotion(ID::Event* e, Game_Screen* _gs)
{
	Cam*	c;
	Game_Screen*	gs;

	gs = (Game_Screen*)_gs;
	c = Cam::GetInstance();
	if (e->motion.x <= 10)
		c->b_left = true;
	else if (e->motion.x > 10)
		c->b_left = false;
	if (e->motion.x >= c->GetCameraWidth() - 10)
		c->b_right = true;
	else if (e->motion.x < c->GetCameraWidth() - 10)
		c->b_right = false;
	if (e->motion.y <= 15)
		c->b_top = true;
	else if (e->motion.y > 15)
		c->b_top = false;
	if (e->motion.y >= Env::GetInstance()->window->GetHeight() - 20)
		c->b_down = true;
	else if (e->motion.y < Env::GetInstance()->window->GetHeight() - 20)
		c->b_down = false;
}

void	GetSelection(ID::GRect* sel)
{
	Protocol_Manager*	pm;
	Game*			gm;
	Cam*			c;
	Map*			map;
	std::list<Unit*>::reverse_iterator	it;
	std::list<Unit*>::reverse_iterator	end;
	int16_t		x;
	int16_t		y;
	uint16_t	w;
	uint16_t	h;
	int16_t		xC;
	int16_t		yC;
	uint16_t	wC;
	uint16_t	hC;
	int		i;
	uint32_t	updateSaver;
	Position	pos;
	ID::Rect	selector;
	ID::Rect	unit;
	bool		newSelection;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return ;

	pm = Protocol_Manager::GetInstance();
	c = Cam::GetInstance();
	map = Map::GetInstance();
	gm = Game::GetInstance();

	sel->GetPos(&x, &y);
	x += c->GetOffsetX();
	y += c->GetOffsetY();
	w = sel->GetWidth() + 1;
	h = sel->GetHeight() + 1;
	std::cout << "GetSelection("
		<< x << ","
		<< y << ")"
		<< w << "|"
		<< h
		<< "\n";
	selector.x = x;
	selector.y = y;
	selector.w = w;
	selector.h = h;
	xC = x >> 5;
	yC = y >> 5;
	wC = (x + w) >> 5;
	if (wC >= map->w)
		wC = map->w - 1;
	hC = (y + h) >> 5;
	if (hC >= map->h)
		hC = map->h - 1;
	i = yC * map->w + xC;
	std::cout << "("
		<< xC << ","
		<< yC << ")"
		<< wC << "|"
		<< hC
		<< "	I=" << i
		<< "\n";
	updateSaver = tv.tv_usec;
	newSelection = true;
	while (yC <= hC)
	{
		xC = x >> 5;
		i = yC * map->w + xC;
		while (xC <= wC)
		{
			it = map->cases[i].units.rbegin();
			end = map->cases[i].units.rend();
			while (it != end)
			{
		//#TODO ajouter une mutexounette.
	//	gm->LockMutex();
				pos = (*it)->GetPosition();
				unit.w = (*it)->GetModel()->GetSize() << 1;
				unit.h = (*it)->GetModel()->GetSize() << 1;
				unit.x = (pos.x * CASE_SIDE_PIXELS) - (*it)->GetModel()->GetSize();
				unit.y = (pos.y * CASE_SIDE_PIXELS) - (*it)->GetModel()->GetSize();
				std::cout << "Unit("
					<< unit.x
					<< ","
					<< unit.y
					<< ","
					<< unit.w
					<< ")"
					<< std::endl;
				if ((*it)->GetUpdateSaver() != updateSaver &&
				    (*it)->GetPlayer() == gm->GetMe() &&
				    CheckCollision_RectRect(&selector, &unit))
				{
					if (newSelection == true)
					{
						gm->lastSelection = gm->currentTick;
						gm->GetMe()->ClearSelection();
						newSelection = false;
						if (selector.w == 1 && selector.h == 1)
						{
							gm->GetMe()->AddUnit2Selection(*it, gm->currentTick);
							pm->prot.AddCmdGameSelection(gm->players[gm->myID]);
							Env::GetInstance()->gameScreen->RefreshSelectedList();
							return ;
						}
					}
					gm->GetMe()->AddUnit2Selection(*it, gm->currentTick);
					std::cout << "Add\n";
				}
	//	gm->UnlockMutex();
				(*it)->SetUpdateSaver(updateSaver);
				++it;
			}
			++i;
			++xC;
		}
		++yC;
	}
	if (gm->GetMe()->GetSelection()->size() > 0)
	{
		std::cout << "AddcmdGameSel\n";
		pm->prot.AddCmdGameSelection(gm->players[gm->myID]);
		Env::GetInstance()->gameScreen->RefreshSelectedList();
	}
}

