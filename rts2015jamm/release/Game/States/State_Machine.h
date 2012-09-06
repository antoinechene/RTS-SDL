#ifndef	__STATE_MACHINE_H__
#define	__STATE_MACHINE_H__

#include <queue>
#include <list>
#include <stdarg.h>
#include <stdint.h>
#include "../../vector.h"
#include "FSM_Event.h"

enum	FSM
{
	FSM_IDLE = 1,
	FSM_ATTACKER
};

enum	STATE
{
	IDLE = 1,
	IDLE_ATTACK,
	MOVE,
	MOVE_ATTACK_AUTO,
	MOVE_ATTACK_FOCUS,
	FOLLOW_ATTACK_AUTO,
	ATTACK_AUTO,
	ATTACK_FOCUS
};

class	State;
class	Unit;
class	GameEvent;

class	State_Machine
{
	public:
			State_Machine(Unit*);
			~State_Machine();
		virtual void	ApplyEvent(FSM_Event*) = 0;

		void	Update(void);
		void	SetNextState(uint32_t);
		void	SwitchState(uint32_t);
		int	SetStates(uint32_t n, ...);
		void	SetInitialState(uint32_t);
		State*	GetState(uint32_t ID);

		inline Unit*	GetUnit(void)
		{ return this->__unit; }
	private:
		Unit*			__unit;
		State*			__currentState;
		int			__nextState;
		ID::vector<State*>	__finiteStatesList;
};

#endif
