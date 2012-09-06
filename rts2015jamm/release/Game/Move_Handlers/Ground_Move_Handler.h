#ifndef	__GROUND_MOVE_HANDLER_H__
#define	__GROUND_MOVE_HANDLER_H__

#include "Move_Handler.h"

class	Node;

class	Ground_Move_Handler : public Move_Handler
{
	public:
				Ground_Move_Handler(Unit*);
				~Ground_Move_Handler();
		virtual bool	CanMove(double x, double y);
		virtual int	Move();

	public:
		int	maxTickBlocked;
		double	initialDist;
		double	deltaX;
		double	deltaY;

	private:
		void		__CalculDelta(double dist, double xU, double yU, Node*);
};

#endif
