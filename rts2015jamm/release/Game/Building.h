#ifndef	__BUILDING_H__
#define __BUILDING_H__

#include <list>
#include "Unit.h"
#include "Production_DB.h"

class	HUD_Production;
class	Building;

struct	Prod_Stack
{
	Production_DB*	prodDB;
	HUD_Production*	LinkedButton;
	int		nb;
	Building*	parent;
};

class	Building : public Unit
{
 private:
	float			_buildingTime;
	std::list<Prod_Stack*>	_productionList;
 public:
			Building(Unit_Model*, Player* = NULL);
			~Building();
	virtual	void	Update(void);
		void	AddToBuildList(Production_DB*);
		int	SpawnBuiltUnit(int builtID);
		int	GetProdPercent();
		bool	IsInProd();
		std::list<Prod_Stack*>*	GetProdList();
};

#endif
