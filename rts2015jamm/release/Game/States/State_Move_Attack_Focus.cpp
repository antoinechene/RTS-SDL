#include "State_Move_Attack_Focus.h"
#include "State_Machine.h"
#include "../Unit.h"
#include "../AI_AStar.h"
#include "../Move_Handlers/Move_Handler.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

#include <iostream>

State_Move_Attack_Focus::State_Move_Attack_Focus() : State(MOVE_ATTACK_FOCUS)
{
}

State_Move_Attack_Focus::~State_Move_Attack_Focus()
{
}

void	State_Move_Attack_Focus::Update(void)
{
	Unit*	u;
	Unit*	target;
	double	xTargetMoveUnit;
	double	yTargetMoveUnit;

	u = this->_motherMachine->GetUnit();
	target = u->GetTargetAttackUnit();
	if (target == NULL)
	{
		this->_motherMachine->SetNextState(IDLE_ATTACK);
		return ;
	}
	if (u->GetAttackHandler()->CanAttack(target) == true)
	{
		this->_motherMachine->SetNextState(ATTACK_FOCUS);
		return ;
	}
	target->GetPositionXY(&xTargetMoveUnit, &yTargetMoveUnit);
	if (this->xT != xTargetMoveUnit ||
	    this->yT != yTargetMoveUnit)
	{
		if (xTargetMoveUnit == -1 && yTargetMoveUnit == -1)
			this->_motherMachine->SetNextState(IDLE_ATTACK);
		else
		{
			this->_motherMachine->SetNextState(MOVE);
		}
		return ;
	}
	u->GetMoveHandler()->Move();
}

void	State_Move_Attack_Focus::OnEnter(void)
{
	std::cout << "OnEnter Move_Attack_Focus\n";
	AI_AStar	as;
	Unit*		u;
	Unit*		target;
	Node*		path;

	//#TODO Check target
	// if (CanAttack)
	// {
	//	Switch ATTACK FOCUS
	//	return ;
	// }
	u = this->_motherMachine->GetUnit();
	if (u->GetModel()->GetMoveSpeed() > 0)
	{
		target = u->GetTargetAttackUnit();
		if (target != NULL)
		{
			target->GetPositionXY(&(this->xT), &(this->yT));
			path = as.Find(u, (int)this->xT, (int)this->yT);
			if (path != NULL)
				u->GetMoveHandler()->SetPath(path);
			this->maxTickBlocked = 0;
			return ;
		}
	}
	u->SetTargetMove(-1, -1);
	this->_motherMachine->SetNextState(IDLE_ATTACK);
}

void	State_Move_Attack_Focus::OnLeave(void)
{
	std::cout << "OnLeave Move_Attack_Focus\n";
	Unit*		u;

	u = this->_motherMachine->GetUnit();
	u->GetMoveHandler()->SetPath(NULL);
}

