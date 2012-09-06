#ifndef	__USE_GROUP_H__
#define	__USE_GROUP_H__

#include "GameEvent.hpp"

class	Use_Group : public GameEvent
{
	public:
			Use_Group(uint8_t idGroup, uint8_t idPlayer, uint32_t t);
		virtual ~Use_Group();
		virtual void	Apply();

	private:
		uint8_t	__idGroup;
		uint8_t	__idPlayer;
};

#endif
