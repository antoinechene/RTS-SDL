#include "State_Attack_Focus.h"
#include "State_Machine.h"
#include "../Game.h"
#include "../Player.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../../Constantes.h"

#include <iostream>

State_Attack_Focus::State_Attack_Focus() : State(ATTACK_FOCUS)
{
}

State_Attack_Focus::~State_Attack_Focus()
{
}

void	State_Attack_Focus::Update(void)
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

void	State_Attack_Focus::OnEnter(void)
{
	std::cout << "OnEnter Attack Focus\n";
	Unit*	u;
	Game*	gm;

	gm = Game::GetInstance();
	u = this->_motherMachine->GetUnit();
	this->targetUnit = u->GetTargetAttackUnit();
	if (this->targetUnit != NULL)
	{
		if (u->GetAttackHandler()->CanAttack(this->targetUnit) == false)
			this->__BackToParent();
	}
	else
		this->__BackToParent();
}

void	State_Attack_Focus::OnLeave(void)
{
	std::cout << "OnLeave Attack Focus\n";
}

void	State_Attack_Focus::__BackToParent(void)
{
	Unit*	t;

	t = this->_motherMachine->GetUnit()->GetTargetAttackUnit();
	if (this->targetUnit != NULL)
	{
		//Follow
	}
	else
		this->_motherMachine->SetNextState(IDLE_ATTACK);
}
