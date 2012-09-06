#include "FSM_Attacker.h"

FSM_Attacker::FSM_Attacker(Unit* u) : State_Machine(u)
{
	this->SetStates(7,
			&(this->_idleAttack),
			&(this->_move),
			&(this->_moveAttackAuto),
			&(this->_moveAttackFocus),
			&(this->_followAttackAuto),
			&(this->_attackAuto),
			&(this->_attackFocus));
	this->SetInitialState(IDLE_ATTACK);
}

FSM_Attacker::~FSM_Attacker()
{
}

void	FSM_Attacker::ApplyEvent(FSM_Event* e)
{
	if (e->type == FSM_ORDER_MOVE)
	{
		this->SetNextState(MOVE);
	}
	else if (e->type == FSM_ORDER_FOLLOW)
	{
		this->SetNextState(FOLLOW_ATTACK_AUTO);
	}
	else if (e->type == FSM_ORDER_HOLD)
	{
		this->SetNextState(IDLE_ATTACK);
	}
	else if (e->type == FSM_ORDER_ATTACK)
	{
		if (e->attack.unit != NULL)
			this->SetNextState(ATTACK_FOCUS);
		else
			this->SetNextState(ATTACK_AUTO);
	}
}

