#ifndef	__STATE_MOVE_H__
#define	__STATE_MOVE_H__

#include "State.h"

class	State_Move : public State
{
	public:
		State_Move();
		~State_Move();

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
