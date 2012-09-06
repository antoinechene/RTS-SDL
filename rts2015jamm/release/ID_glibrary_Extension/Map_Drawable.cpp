#include "Map_Drawable.h"
#include "../Constantes.h"
#include "../Game/Cam.h"
#include "../Game/Game.h"
#include "../Game/Map.h"
#include "../Game/Unit.h"
#include "../Game/Player.h"
#include <iostream>

void	__Map_Drawable__CbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "MD_ClickDown" << std::endl;
}

Map_Drawable::Map_Drawable(int level, uint16_t w, uint16_t h)
{
	this->SetName("Map_Drawable");
	this->_grounds.resize(level);

	this->_w = w;
	this->_h = h;
	this->SetCbClickDown(__Map_Drawable__CbClickDown, NULL);
}

Map_Drawable::~Map_Drawable()
{
	this->_grounds.clear();
}

void	Map_Drawable::AddGround(ID::Drawable* d, int level)
{
	this->_grounds[level].push_back(d);
	d->SetParent(this);
//	this->SetInvalidate(true);
}

void	Map_Drawable::DelGround(ID::Drawable* d, int level)
{
	std::vector<ID::Drawable*>::iterator	it;
	std::vector<ID::Drawable*>::iterator	end;

	it = this->_grounds[level].begin();
	end = this->_grounds[level].end();
	while (it != end)
	{
		if (*it == d)
		{
			this->_grounds[level].erase(it);
			break ;
		}
		++it;
	}
//	this->SetInvalidate(true);
}

void	Map_Drawable::ClearGround(int level)
{
/*	std::vector<ID::Drawable*>::iterator	it;
	std::vector<ID::Drawable*>::iterator	end;

	it = this->_grounds[level].begin();
	end = this->_grounds[level].end();
	while (it != end)
	{
		(*it)->SetParent(NULL);
		++it;
	}*/
	this->_grounds[level].clear();
	this->SetInvalidate(true);
}

std::vector<ID::Drawable*>*
	Map_Drawable::GetGround(int level)
{
	return &(this->_grounds[level]);
}

int	Map_Drawable::RefreshToSurface(ID::Surface* s,
				       int x, int y)
{
	Game*					g;
	Cam*					c;
	Map*					m;
	Light_Drawable*				cases;
	uint32_t				i;
//	uint32_t				indexSaver;
	uint32_t				tmpSaver;
	std::list<Unit*>::iterator		itU;
	std::list<Unit*>::iterator		endU;
	int					xMin;
	int					yMin;
	int					xMax;
	int					yMax;
	int					offsetCamX;
	int					offsetCamY;
	ID::Draw_Geometry_Circle*		focus;
	double					xU;
	double					yU;
	bool					selected;

	g = Game::GetInstance();
	c = Cam::GetInstance();
	m = Map::GetInstance();
	focus = ID::Draw_Geometry_Circle::GetInstance();

	if (this->__RefreshGrounds(UNDER_BACKGROUND, ON_BACKGROUND, s, x, y) == -1)
		return -1;

	cases = c->GetCases();
	offsetCamX = c->GetOffsetX();
	offsetCamY = c->GetOffsetY();
	xMax = c->GetCameraWidth() / CASE_SIDE_PIXELS;
	yMax = c->GetCameraHeight() / CASE_SIDE_PIXELS;
//	indexSaver = ((c->GetY() / CASE_SIDE_PIXELS) -
//		      (c->GetCameraHeightRayon() / CASE_SIDE_PIXELS)) *
//		     (c->GetMapWidth() / CASE_SIDE_PIXELS) +
//		     (c->GetX() / CASE_SIDE_PIXELS - c->GetCameraWidthRayon() / CASE_SIDE_PIXELS);
	tmpSaver = g->currentTick;
	yMin = 0;
	while (yMin < yMax)
	{
		xMin = 0;
		i = ((yMin + (c->GetOffsetY() / CASE_SIDE_PIXELS)) *
		     (c->GetMapWidth() / CASE_SIDE_PIXELS)) +
		    (c->GetOffsetX() / CASE_SIDE_PIXELS);
		while (xMin < xMax)
		{
			itU = m->cases[i].units.begin();
			endU = m->cases[i].units.end();
			while (itU != endU)
			{
				if (((*itU)->GetModel()->GetType() == TYPE_GROUND ||
				     (*itU)->GetModel()->GetType() == 3) &&
				    (*itU)->GetUpdateSaver() != tmpSaver)
				{
					(*itU)->SetUpdateSaver(tmpSaver);
					(*itU)->GetPositionXY(&xU, &yU);
					selected = (*itU)->GetTickSelection() == g->lastSelection;
					focus->Draw(s,
						    xU * CASE_SIDE_PIXELS - offsetCamX,
						    yU * CASE_SIDE_PIXELS - offsetCamY,
						    (*itU)->GetModel()->GetSize(),
						    selected,
						    &(m->_colorsPalette[(*itU)->GetPlayer()->GetID()]));
				}
				++itU;
			}
			++xMin;
			++i;
		}
		++yMin;
	}

	if (this->__RefreshGrounds(MIDDLEGROUND, MIDDLEGROUND, s, x, y) == -1)
		return -1;

	tmpSaver = g->currentTick + 1;
//	i = indexSaver;
	yMin = 0;
//	std::cout << "- - - - - - - - \n";
	while (yMin < yMax)
	{
		xMin = 0;
		i = ((yMin + (c->GetOffsetY() / CASE_SIDE_PIXELS)) *
		     (c->GetMapWidth() / CASE_SIDE_PIXELS)) +
		    (c->GetOffsetX() / CASE_SIDE_PIXELS);
		while (xMin < xMax)
		{
			itU = m->cases[i].units.begin();
			endU = m->cases[i].units.end();
//			std::cout << i << "	";
			while (itU != endU)
			{
				if (((*itU)->GetModel()->GetType() == TYPE_AIR ||
				     (*itU)->GetModel()->GetType() == 4) &&
				    (*itU)->GetUpdateSaver() != tmpSaver)
				{
//					std::cout << "|";
					(*itU)->SetUpdateSaver(tmpSaver);
					(*itU)->GetPositionXY(&xU, &yU);
					selected = ((*itU)->GetTickSelection() == g->lastSelection);
					focus->Draw(s,
						    xU * CASE_SIDE_PIXELS - offsetCamX,
						    yU * CASE_SIDE_PIXELS - offsetCamY,
						    (*itU)->GetModel()->GetSize(),
						    selected,
						    &(m->_colorsPalette[(*itU)->GetPlayer()->GetID()]));
				}
				++itU;
			}
			++xMin;
			++i;
		}
//		std::cout << "\n";
		++yMin;
	}

	if (this->__RefreshGrounds(HIGHGROUND, HIGHGROUND, s, x, y) == -1)
		return -1;

	tmpSaver = g->currentTick - 1;
	//i = indexSaver;
	yMin = 0;
	while (yMin < yMax)
	{
		i = ((yMin + (c->GetOffsetY() / CASE_SIDE_PIXELS)) *
		     (c->GetMapWidth() / CASE_SIDE_PIXELS)) +
		    (c->GetOffsetX() / CASE_SIDE_PIXELS);
		xMin = 0;
		while (xMin < xMax)
		{
			itU = m->cases[i].units.begin();
			endU = m->cases[i].units.end();
			while (itU != endU)
			{
				if ((*itU)->GetUpdateSaver() != tmpSaver)
				{
					(*itU)->SetUpdateSaver(tmpSaver);
					(*itU)->GetPositionXY(&xU, &yU);
					this->__RefreshBars(s,
							    xU * CASE_SIDE_PIXELS - offsetCamX - (*itU)->GetModel()->GetSize(),
							    yU * CASE_SIDE_PIXELS - offsetCamY - (*itU)->GetModel()->GetSize() - 5,
							    (*itU)->GetModel()->GetSize(),
							    (*itU)->GetHP(),
							    (*itU)->GetModel()->GetHPMax());
				}
				++itU;
			}
			++xMin;
			++i;
		}
		++yMin;
	}

	if (this->__RefreshGrounds(FOREGROUND, FOREGROUND, s, x, y) == -1)
		return -1;

	this->SetInvalidate(false);
	return 0;
}

int	Map_Drawable::__RefreshGrounds(int min, int max, ID::Surface* s, int x, int y)
{
	std::vector<ID::Drawable*>::iterator	it;
	std::vector<ID::Drawable*>::iterator	end;

	while (min <= max)
	{
		it = this->_grounds[min].begin();
		end = this->_grounds[min].end();
		while (it != end)
		{
			if ((*it)->RefreshToSurface(s, x, y) == -1)
				return -1;
			++it;
		}
		++min;
	}
	return 0;
}

int	Map_Drawable::__RefreshBars(ID::Surface* s, int x, int y, int size, int hp, int maxHP)
{
	ID::Draw_Geometry_Rect*			bar;
	int					maxSize;

	if (x < 0 || y < 0)
		return -1;
	maxSize = size << 1;
	bar = ID::Draw_Geometry_Rect::GetInstance();
	hp = hp * maxSize / maxHP;
	bar->Draw(s,
		  x,
		  y,
		  hp,
		  5,
		  true,
		  ID::COLOR_GREEN.ConvertToInt());
	if (hp != maxSize)
	{
		bar->Draw(s,
			  x + hp,
			  y,
			  maxSize - hp,
			  5,
			  true,
			  ID::COLOR_RED.ConvertToInt());
	}
	return 0;
}

