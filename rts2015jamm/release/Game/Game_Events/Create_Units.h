#ifndef	__CREATE_UNITS_H__
#define	__CREATE_UNITS_H__

#include "GameEvent.hpp"
#include <vector>

class	cmd_Create_Units;

class	Create_Units : public GameEvent
{
	public:
			Create_Units(std::vector<cmd_Create_Units>*, uint32_t t);
		virtual ~Create_Units();
		virtual void	Apply();

	private:
		std::vector<cmd_Create_Units>*	__cmd;
};

#endif
