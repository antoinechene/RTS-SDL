#include "FSM_Idle.h"

FSM_Idle::FSM_Idle(Unit* u) : State_Machine(u)
{
	this->SetStates(1, &(this->_idle));
	this->SetInitialState(IDLE);
}

FSM_Idle::~FSM_Idle()
{
}

void	FSM_Idle::ApplyEvent(FSM_Event*)
{
}

