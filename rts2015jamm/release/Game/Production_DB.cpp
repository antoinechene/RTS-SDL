#include "Production_DB.h"

Production_DB::Production_DB(char** tab)
{
	Database*	db;
	int		i;
	Resource	res;

	db = Database::GetInstance();
	this->builderID = atoi(tab[0]);
	this->prodID = atoi(tab[1]);
	this->prodTime = atoi(tab[2]);
	this->nbResources = atoi(tab[3]);
	i = 0;
	while (i < this->nbResources)
	{
		res.model = db->getResourceModel(atoi(tab[4 + (i << 1)]));
		res.qty = atoi(tab[5 + (i << 1)]);
		std::cout << "prod.qty" << res.qty << "\n";
		this->resourcesList.push_back(res);
		++i;
	}
}

Production_DB::~Production_DB()
{
}
