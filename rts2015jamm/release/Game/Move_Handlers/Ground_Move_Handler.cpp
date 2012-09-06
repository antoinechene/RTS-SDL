#include "Ground_Move_Handler.h"
#include "../Game.h"
#include "../Unit.h"
#include "../Player.h"
#include "../Map.h"
#include "../AI_AStar.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

Ground_Move_Handler::Ground_Move_Handler(Unit* u) : Move_Handler(u)
{
}

Ground_Move_Handler::~Ground_Move_Handler()
{
}

bool	Ground_Move_Handler::CanMove(double x, double y)
{
	Game*				gm;
	Map*				map;
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;
	std::vector<Unit*>		selection;
	int16_t		_x;
	int16_t		_y;
	uint16_t	w;
	uint16_t	h;
	int16_t		xC;
	int16_t		yC;
	uint16_t	wC;
	uint16_t	hC;
	int		i;

	ID::Circle	c;
	ID::Rect	r;

	map = Map::GetInstance();
	gm = Game::GetInstance();

	r.w = CASE_SIDE_PIXELS;
	r.h = CASE_SIDE_PIXELS;

	_x = (int16_t)(x * r.w);
	_y = (int16_t)(y * r.h);

	c.center.x = _x;
	c.center.y = _y;
	c.radius = this->_unit->GetModel()->GetSize();

	w = _x + this->_unit->GetModel()->GetSize();
	h = _y + this->_unit->GetModel()->GetSize();
	_x -= this->_unit->GetModel()->GetSize();
	_y -= this->_unit->GetModel()->GetSize();
	if (_x < 0)
		_x = 0;
	if (_y < 0)
		_y = 0;

	xC = _x >> 5;
	yC = _y >> 5;
	wC = w >> 5;
	hC = h >> 5;
	i = yC * map->w + xC;
	while (yC <= hC)
	{
		xC = _x >> 5;
		i = yC * map->w + xC;
		while (xC <= wC)
		{
			r.x = (xC << 5) + (r.w >> 1);
			r.y = (yC << 5) + (r.h >> 1);
			//#TODO Add Collision with unit or not...
			if (map->cases[i].
				animation.
				GetSpriteAnime()->
				caract->
				access == 0)
				return false;
			++i;
			++xC;
		}
		++yC;
	}
	return true;
}

int	Ground_Move_Handler::Move()
{
	Unit*	u;
	Node*	n;
	Node*	last;
	double	distMax;
	double	xU;
	double	yU;
	double	nX;
	double	nY;
	bool	recalcul;

	u = this->_unit;
	n = u->GetMoveHandler()->GetPath();
	if (n != NULL)
	{
		distMax = u->GetModel()->GetMoveSpeed() / MAX_TPS;
		u->GetPositionXY(&xU, &yU);
		recalcul = true;
		while (distMax > 0.0001)
		{
			last = NULL;
			while (n->parent != NULL)
			{
				last = n;
				n = n->parent;
			}
			if (recalcul == true)
			{
				this->__CalculDelta(distMax, xU, yU, n);
				recalcul = false;
			}
			if (deltaX >= 0)
			{
				if (xU + this->deltaX < n->x)
					nX = xU + this->deltaX;
				else
					nX = n->x;
			}
			else
			{
				if (xU + this->deltaX > n->x)
					nX = xU + this->deltaX;
				else
					nX = n->x;
			}
			if (deltaY >= 0)
			{
				if (yU + this->deltaY < n->y)
					nY = yU + this->deltaY;
				else
					nY = n->y;
			}
			else
			{
				if (yU + this->deltaY > n->y)
					nY = yU + this->deltaY;
				else
					nY = n->y;
			}
			if (u->GetMoveHandler()->CanMove(nX, nY) == false)
			{
				++this->maxTickBlocked;
				if (this->maxTickBlocked == 5)
					u->SetTargetMove(-1, -1);
				return 0;
			}
			this->maxTickBlocked = 0;
			distMax -= IDdistance(xU, yU, nX, nY);

		/*	std::cout << "("
				<< xU
				<< ";"
				<< yU
				<< ") + ("
				<< this->deltaX
				<< ";"
				<< this->deltaY
				<< ") = ("
				<< nX
				<< ";"
				<< nY
				<< ")	"
				<< distMax
				<< std::endl;
		*/	if (nX == n->x && nY == n->y)
			{
				if (last != NULL)
				{
					last->parent = NULL;
					delete n;
					n = u->GetMoveHandler()->GetPath();
					recalcul = true;
				}
				else
				{
					u->SetTargetMove(-1, -1);
					break ;
				}
			}
			xU = nX;
			yU = nY;
		}
		u->SetPosition(nX, nY);
	}
	return 0;
}

void	Ground_Move_Handler::__CalculDelta(double distMax, double xU, double yU, Node* n)
{
	double	speedByTick;

	speedByTick = distMax / IDdistance(xU, yU, n->x, n->y);
	this->deltaX = speedByTick * (n->x - xU);
	this->deltaY = speedByTick * (n->y - yU);
}
