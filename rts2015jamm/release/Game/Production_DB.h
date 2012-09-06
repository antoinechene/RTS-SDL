#ifndef __PRODUCTION_DB_H__
#define __PRODUCTION_DB_H__

#include <vector>
#include <stdlib.h>
#include "Player.h"
#include "../Tools.h"
#include "../Database/Database.h"

class	Production_DB
{
 public:
	int			builderID;
	int			prodID;
	int			prodTime;
	int			nbResources;
	std::vector<Resource>	resourcesList;
 public:
	Production_DB(char** tab);
	~Production_DB();
};

#endif
