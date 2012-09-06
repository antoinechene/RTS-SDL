#ifndef	__STATE_ATTACK_AUTO_H__
#define	__STATE_ATTACK_AUTO_H__

#include "State.h"
#include "../Unit.h"

class	State_Attack_Auto : public State
{
	public:
		State_Attack_Auto();
		~State_Attack_Auto();

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
