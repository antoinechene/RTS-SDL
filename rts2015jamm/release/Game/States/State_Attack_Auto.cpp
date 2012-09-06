#include "State_Attack_Auto.h"
#include "State_Machine.h"
#include "../Game.h"
#include "../Player.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../../Constantes.h"

#include <iostream>

State_Attack_Auto::State_Attack_Auto() : State(ATTACK_AUTO)
{
}

State_Attack_Auto::~State_Attack_Auto()
{
}

void	State_Attack_Auto::Update(void)
{
	this->targetUnit = this->targetUnit->IsAlive();
	if (this->targetUnit != NULL)
	{
		if (this->_motherMachine->GetUnit()->GetAttackHandler()->Attack(this->targetUnit) != 1)
			this->__BackToParent();
	}
	else
		this->__BackToParent();
}

void	State_Attack_Auto::OnEnter(void)
{
	std::cout << "OnEnter Attack Auto\n";
	Game*	gm;
	Unit*	u;

	gm = Game::GetInstance();
	u = this->_motherMachine->GetUnit();
	this->targetUnit = u->GetTargetAttackAuto();
	if (this->targetUnit != NULL)
	{
		if (u->GetAttackHandler()->CanAttack(this->targetUnit) == false)
			this->__BackToParent();
	}
	else
		this->__BackToParent();
}

void	State_Attack_Auto::OnLeave(void)
{
	std::cout << "OnLeave Attack Auto\n";
}

void	State_Attack_Auto::__BackToParent(void)
{
	double	x;
	double	y;

	this->_motherMachine->GetUnit()->GetTargetAttackPos(&x, &y);
	if (x == -1 && y == -1)
		this->_motherMachine->SetNextState(IDLE_ATTACK);
	else if (this->_motherMachine->GetUnit()->GetTargetFollow() != NULL)
		this->_motherMachine->SetNextState(FOLLOW_ATTACK_AUTO);
	else
		this->_motherMachine->SetNextState(MOVE_ATTACK_AUTO);
	//#TODO Question, on suit l'ennemi ou pas?
}
