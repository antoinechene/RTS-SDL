#ifndef	__STATE_IDLE_ATTACK_H__
#define	__STATE_IDLE_ATTACK_H__

#include "State.h"

class	State_Idle_Attack : public State
{
	public:
		State_Idle_Attack();
		~State_Idle_Attack();

		virtual void	Update(void);
		virtual void	OnEnter(void);
		virtual void	OnLeave(void);
};

#endif
