#ifndef	__STATE_IDLE_H__
#define	__STATE_IDLE_H__

#include "State.h"

class	State_Idle : public State
{
	public:
		State_Idle();
		~State_Idle();

		virtual void	Update(void);
		virtual void	OnEnter(void);
		virtual void	OnLeave(void);
};

#endif
