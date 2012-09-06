#ifndef	__DELETE_UNITS_H__
#define	__DELETE_UNITS_H__

#include "GameEvent.hpp"
#include <vector>

class	dataUnit;

class	Delete_Units : public GameEvent
{
	public:
			Delete_Units(std::vector<dataUnit>*, uint32_t t);
		virtual ~Delete_Units();
		virtual void	Apply();

	private:
		std::vector<dataUnit>*	__cmd;
};

#endif
