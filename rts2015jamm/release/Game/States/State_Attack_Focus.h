#ifndef	__STATE_ATTACK_FOCUS_H__
#define	__STATE_ATTACK_FOCUS_H__

#include "State.h"
#include "../Unit.h"

class	State_Attack_Focus : public State
{
	public:
		State_Attack_Focus();
		~State_Attack_Focus();

		virtual void	Update(void);
		virtual void	OnEnter(void);
		virtual void	OnLeave(void);

	public:
		Unit*		targetUnit;
		Position	targetPos;

	private:
		void	__BackToParent(void);
};

#endif
