#include "State_Move_Attack_Auto.h"
#include "State_Machine.h"
#include "../Unit.h"
#include "../AI_AStar.h"
#include "../Move_Handlers/Move_Handler.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

#include <iostream>

State_Move_Attack_Auto::State_Move_Attack_Auto() : State(MOVE_ATTACK_AUTO)
{
}

State_Move_Attack_Auto::~State_Move_Attack_Auto()
{
}

void	State_Move_Attack_Auto::Update(void)
{
	Unit*	u;
	Unit*	target;
	double	xTargetMoveUnit;
	double	yTargetMoveUnit;

	u = this->_motherMachine->GetUnit();
	target = u->GetTargetAttackAuto();
	if (target != NULL)
	{
		this->_motherMachine->SetNextState(ATTACK_AUTO);
	//	Update();
		return ;
	}

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

void	State_Move_Attack_Auto::OnEnter(void)
{
	std::cout << "OnEnter Move_Attack_Auto\n";
	AI_AStar	as;
	Unit*		u;
	Node*		path;

	u = this->_motherMachine->GetUnit();
	if (u->GetModel()->GetMoveSpeed() > 0)
	{
		u->GetTargetAttackPos(&(this->xT), &(this->yT));
		path = as.Find(u, (int)this->xT, (int)this->yT);
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

void	State_Move_Attack_Auto::OnLeave(void)
{
	std::cout << "OnLeave Move_Attack_Auto\n";
	Unit*		u;

	u = this->_motherMachine->GetUnit();
	u->GetMoveHandler()->SetPath(NULL);
}

