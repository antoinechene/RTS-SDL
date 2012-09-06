#ifndef	__GROUP_H__
#define	__GROUP_H__

#include <stdint.h>
#include <list>

class	Unit;

class	Group
{
	public:
		Group();
		~Group();

		void			AddUnit(Unit* unit);
		void			DelUnit(Unit* unit);
		void			ClearUnits(void);
		std::list<Unit*>*	GetUnits(void);

	public:
		uint32_t	id;

	protected:
		std::list<Unit*>	_units;
};

#endif
