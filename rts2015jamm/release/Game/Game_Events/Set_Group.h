#ifndef	__SET_GROUP_H__
#define	__SET_GROUP_H__

#include "GameEvent.hpp"
#include <vector>

class	cmd_Set_Group;

class	Set_Group : public GameEvent
{
	public:
			Set_Group(std::vector<uint8_t>*, uint8_t idPlayer, uint32_t t);
		virtual ~Set_Group();
		virtual void	Apply();

	private:
		std::vector<uint8_t>*	__cmd;
		uint8_t			__idPlayer;
};

#endif
