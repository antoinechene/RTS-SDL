#include "Air_Move_Handler.h"
#include "../Game.h"
#include "../Unit.h"
#include "../Player.h"
#include "../Map.h"
#include "../AI_AStar.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

Air_Move_Handler::Air_Move_Handler(Unit* u) : Move_Handler(u)
{
}

Air_Move_Handler::~Air_Move_Handler()
{
}

bool	Air_Move_Handler::CanMove(double, double)
{
	return true;
}

int	Air_Move_Handler::Move()
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
			distMax -= IDdistance(xU, yU, nX, nY);

			if (nX == n->x && nY == n->y)
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

void	Air_Move_Handler::__CalculDelta(double distMax, double xU, double yU, Node* n)
{
	double	speedByTick;

	speedByTick = distMax / IDdistance(xU, yU, n->x, n->y);
	this->deltaX = speedByTick * (n->x - xU);
	this->deltaY = speedByTick * (n->y - yU);
}
