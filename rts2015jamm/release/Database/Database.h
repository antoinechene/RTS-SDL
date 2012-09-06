#ifndef	__DATABASE_H__
#define	__DATABASE_H__

#include "../Singleton.hpp"
#include <vector>

class	Unit_Model;
class	Resource_Model;
class	Production_DB;

class	Database : public Singleton<Database>
{
	friend class	Singleton<Database>;
	public:
				Database();
				~Database();
		int		Init();
		int		Init_UnitDB();
		int		Init_ResourceDB();
		int		Init_ProductionDB();
		Unit_Model*	getUnitModel(int id);
		Resource_Model*	getResourceModel(int id);
		std::vector<Production_DB*>	getProductionList(int builder_id);
	public:
		std::vector<Unit_Model*>	unitDB;
		std::vector<Resource_Model*>	resourceDB;
		std::vector<Production_DB*>	prodDB;
};

#endif
