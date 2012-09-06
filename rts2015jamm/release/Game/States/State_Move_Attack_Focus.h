#ifndef	__STATE_MOVE_ATTACK_FOCUS_H__
#define	__STATE_MOVE_ATTACK_FOCUS_H__

#include "State.h"

class	State_Move_Attack_Focus : public State
{
	public:
		State_Move_Attack_Focus();
		~State_Move_Attack_Focus();

		virtual void	Update(void);
		virtual void	OnEnter(void);
		virtual void	OnLeave(void);

		int	maxTickBlocked;
		double	initialDist;
		double	speedByTick;
		double	deltaX;
		double	deltaY;
		bool	recalcul;

		double	xT;
		double	yT;
};

#endif
