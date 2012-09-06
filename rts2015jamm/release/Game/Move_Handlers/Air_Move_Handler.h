#ifndef	__AIR_MOVE_HANDLER_H__
#define	__AIR_MOVE_HANDLER_H__

#include "Move_Handler.h"

class	Node;

class	Air_Move_Handler : public Move_Handler
{
	public:
				Air_Move_Handler(Unit*);
				~Air_Move_Handler();
		virtual bool	CanMove(double x, double y);
		virtual int	Move();

	public:
		double	initialDist;
		double	deltaX;
		double	deltaY;

	private:
		void		__CalculDelta(double dist, double xU, double yU, Node*);
};

#endif
