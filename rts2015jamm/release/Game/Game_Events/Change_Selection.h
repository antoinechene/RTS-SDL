#ifndef	__CHANGE_SELECTION_H__
#define	__CHANGE_SELECTION_H__

#include "GameEvent.hpp"
#include <vector>

class	Change_Selection : public GameEvent
{
	public:
				Change_Selection(std::vector<uint32_t>*, uint8_t, uint32_t);
		virtual		~Change_Selection();
		virtual void	Apply();

	private:
		std::vector<uint32_t>*	__cmd;
		uint8_t			__player;
};

#endif
