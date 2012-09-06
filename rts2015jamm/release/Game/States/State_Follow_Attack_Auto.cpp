#include "State_Follow_Attack_Auto.h"
#include "State_Machine.h"
#include "../Unit.h"
#include "../AI_AStar.h"
#include "../Move_Handlers/Move_Handler.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../../Constantes.h"
#include "../Math_tools.h"

#include <iostream>

State_Follow_Attack_Auto::State_Follow_Attack_Auto() : State(FOLLOW_ATTACK_AUTO)
{
}

State_Follow_Attack_Auto::~State_Follow_Attack_Auto()
{
}

void	State_Follow_Attack_Auto::Update(void)
{
	Unit*	u;
	Unit*	target;
	double	xTargetFollowUnit;
	double	yTargetFollowUnit;
	double	dX;
	double	dY;

	u = this->_motherMachine->GetUnit();
	target = u->GetTargetAttackAuto();
	if (target != NULL)
	{
		this->_motherMachine->SetNextState(ATTACK_AUTO);
	//	Update();
		return ;
	}

	target = u->GetTargetFollow();
	if (target == NULL)
	{
		this->_motherMachine->SetNextState(IDLE_ATTACK);
		return ;
	}
	target->GetPositionXY(&xTargetFollowUnit, &yTargetFollowUnit);
	dX = ABS(this->xT - xTargetFollowUnit);
	dY = ABS(this->yT - yTargetFollowUnit);
	if (dX > 2 ||
	    dY > 2)
	{
		this->_motherMachine->SetNextState(FOLLOW_ATTACK_AUTO);
		return ;
	}
	u->GetMoveHandler()->Move();
}

void	State_Follow_Attack_Auto::OnEnter(void)
{
	std::cout << "OnEnter Follow_Attack_Auto\n";
	AI_AStar	as;
	Unit*		u;
	Unit*		target;
	Node*		path;

	u = this->_motherMachine->GetUnit();
	if (u->GetModel()->GetMoveSpeed() > 0)
	{
		target = u->GetTargetFollow();
		if (target != NULL)
		{
			target->GetPositionXY(&(this->xT), &(this->yT));
			path = as.Find(u, (int)this->xT, (int)this->yT);
			if (path != NULL)
				u->GetMoveHandler()->SetPath(path);
			this->maxTickBlocked = 0;
		}
	}
	else
		u->SetTargetFollow(NULL);
	this->_motherMachine->SetNextState(IDLE_ATTACK);
}

void	State_Follow_Attack_Auto::OnLeave(void)
{
	std::cout << "OnLeave Follow_Attack_Auto\n";
	Unit*		u;

	u = this->_motherMachine->GetUnit();
	u->GetMoveHandler()->SetPath(NULL);
}

