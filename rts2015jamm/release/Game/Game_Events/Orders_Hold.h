#ifndef	__ORDERS_HOLD_H__
#define	__ORDERS_HOLD_H__

#include "GameEvent.hpp"
#include <vector>

class	dataUnit;

class	Orders_Hold : public GameEvent
{
	public:
			Orders_Hold(std::vector<dataUnit>*, uint32_t t);
		virtual ~Orders_Hold();
		virtual void	Apply();

	private:
		std::vector<dataUnit>*	__cmd;
};

#endif
