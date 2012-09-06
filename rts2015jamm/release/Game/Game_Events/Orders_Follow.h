#ifndef	__ORDERS_FOLLOW_H__
#define	__ORDERS_FOLLOW_H__

#include "GameEvent.hpp"
#include <vector>
class	cmd_Orders_Follow;

class	Orders_Follow : public GameEvent
{
	public:
			Orders_Follow(std::vector<cmd_Orders_Follow>*, uint32_t t);
		virtual ~Orders_Follow();
		virtual void	Apply();

	private:
		std::vector<cmd_Orders_Follow>*	__cmd;
};

#endif
