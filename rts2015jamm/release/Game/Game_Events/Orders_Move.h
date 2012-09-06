#ifndef	__ORDERS_MOVE_H__
#define	__ORDERS_MOVE_H__

#include "GameEvent.hpp"
#include <vector>
class	cmd_Orders_Move;

class	Orders_Move : public GameEvent
{
	public:
			Orders_Move(std::vector<cmd_Orders_Move>*, uint32_t t);
		virtual ~Orders_Move();
		virtual void	Apply();

	private:
		std::vector<cmd_Orders_Move>*	__cmd;
};

#endif
