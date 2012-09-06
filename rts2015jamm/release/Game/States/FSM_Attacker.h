#ifndef	__FSM_ATTACKER_H__
#define	__FSM_ATTACKER_H__

#include "State_Machine.h"
#include "State_Idle_Attack.h"
#include "State_Move.h"
#include "State_Move_Attack_Auto.h"
#include "State_Move_Attack_Focus.h"
#include "State_Follow_Attack_Auto.h"
#include "State_Attack_Auto.h"
#include "State_Attack_Focus.h"

class	Unit;

class	FSM_Attacker : public State_Machine
{
	public:
		FSM_Attacker(Unit*);
		~FSM_Attacker();
		virtual void	ApplyEvent(FSM_Event*);

	protected:
		State_Idle_Attack	_idleAttack;
		State_Move		_move;
		State_Move_Attack_Auto	_moveAttackAuto;
		State_Move_Attack_Focus	_moveAttackFocus;
		State_Follow_Attack_Auto	_followAttackAuto;
		State_Attack_Auto	_attackAuto;
		State_Attack_Focus	_attackFocus;
};

#endif
