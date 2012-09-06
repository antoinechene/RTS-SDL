#ifndef	__FSM_IDLE_H__
#define	__FSM_IDLE_H__

#include "State_Machine.h"
#include "State_Idle.h"

class	Unit;

class	FSM_Idle : public State_Machine
{
	public:
		FSM_Idle(Unit*);
		~FSM_Idle();
		virtual void	ApplyEvent(FSM_Event*);

	protected:
		State_Idle	_idle;
};

#endif
