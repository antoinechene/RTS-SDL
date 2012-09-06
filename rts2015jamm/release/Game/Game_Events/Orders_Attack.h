#ifndef	__ORDERS_ATTACK_H__
#define	__ORDERS_ATTACK_H__

#include "GameEvent.hpp"
#include <vector>
class	cmd_Orders_Attack;

class	Orders_Attack : public GameEvent
{
	public:
			Orders_Attack(std::vector<cmd_Orders_Attack>*, uint32_t t);
		virtual ~Orders_Attack();
		virtual void	Apply();

	private:
		std::vector<cmd_Orders_Attack>*	__cmd;
};

#endif
