#include "State_Move.h"
#include "State_Machine.h"
#include "../Unit.h"
#include "../AI_AStar.h"
#include "../Move_Handlers/Move_Handler.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

#include <iostream>

State_Move::State_Move() : State(MOVE)
{
}

State_Move::~State_Move()
{
}

void	State_Move::Update(void)
{
	Unit*	u;
	double	xTargetMoveUnit;
	double	yTargetMoveUnit;

	u = this->_motherMachine->GetUnit();
	u->GetTargetMove(&xTargetMoveUnit, &yTargetMoveUnit);
	if (this->xT != xTargetMoveUnit ||
	    this->yT != yTargetMoveUnit) // Change direction
	{
		if (xTargetMoveUnit == -1 && yTargetMoveUnit == -1)
			this->_motherMachine->SetNextState(IDLE_ATTACK);
		else
			this->_motherMachine->SetNextState(MOVE);
		return ;
	}
	u->GetMoveHandler()->Move();
}

void	State_Move::OnEnter(void)
{
	std::cout << "OnEnter Move\n";
	AI_AStar	as;
	Unit*		u;
	Node*		path;

	u = this->_motherMachine->GetUnit();
	if (u->GetModel()->GetMoveSpeed() > 0)
	{
		u->GetTargetMove(&(this->xT), &(this->yT));
		path = as.Find(u, this->xT, this->yT);
		if (path != NULL)
			u->GetMoveHandler()->SetPath(path);
		this->maxTickBlocked = 0;
	}
	else
	{
		u->SetTargetMove(-1, -1);
		this->_motherMachine->SetNextState(IDLE_ATTACK);
	}
}

void	State_Move::OnLeave(void)
{
	std::cout << "OnLeave Move\n";
	Unit*		u;

	u = this->_motherMachine->GetUnit();
	u->GetMoveHandler()->SetPath(NULL);
}

