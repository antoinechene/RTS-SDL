#ifndef	__GAMEEVEMT_HPP_
#define	__GAMEEVEMT_HPP_

#include <stdint.h>

class	GameEvent
{
	friend class	State_Machine;
	public:
				GameEvent(uint32_t t) : tick(t)
				{
				}
		virtual		~GameEvent()
		{
		}
		virtual void	Apply() = 0;

	public:
		uint32_t	tick;

	private:
		uint32_t	__references;
};

#endif

