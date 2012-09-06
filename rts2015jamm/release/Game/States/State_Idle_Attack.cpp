#include "State_Idle_Attack.h"
#include "State_Machine.h"
#include "../Map.h"
#include "../Cam.h"
#include "../Unit.h"
#include "../Attack_Handlers/Attack_Handler.h"
#include "../ChecksCollision.h"

#include <iostream>

State_Idle_Attack::State_Idle_Attack() : State(IDLE_ATTACK)
{
}

State_Idle_Attack::~State_Idle_Attack()
{
}

void	State_Idle_Attack::Update(void)
{
	Unit*	target;

	target = this->_motherMachine->GetUnit()->GetTargetAttackAuto();
	if (target != NULL)
		this->_motherMachine->SetNextState(ATTACK_AUTO);
}

void	State_Idle_Attack::OnEnter(void)
{
	std::cout << "OnEnter Idle Attack\n";
}

void	State_Idle_Attack::OnLeave(void)
{
	std::cout << "OnLeave Idle Attack\n";
}

