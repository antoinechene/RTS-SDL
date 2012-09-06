#ifndef	__STATE_MOVE_ATTACK_AUTO_H__
#define	__STATE_MOVE_ATTACK_AUTO_H__

#include "State.h"

class	State_Move_Attack_Auto : public State
{
	public:
		State_Move_Attack_Auto();
		~State_Move_Attack_Auto();

		virtual void	Update(void);
		virtual void	OnEnter(void);
		virtual void	OnLeave(void);

	public:
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
