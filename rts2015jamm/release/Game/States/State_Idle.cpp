#include "State_Idle.h"
#include "State_Machine.h"
#include "../Unit.h"
#include "../Move_Handlers/Move_Handler.h"

#include <iostream>

State_Idle::State_Idle() : State(IDLE)
{
}

State_Idle::~State_Idle()
{
}

void	State_Idle::Update(void)
{
}

void	State_Idle::OnEnter(void)
{
	std::cout << "OnEnter IDLE\n";
}

void	State_Idle::OnLeave(void)
{
	std::cout << "OnLeave IDLE\n";
}

