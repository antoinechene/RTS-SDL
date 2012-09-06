#include "../Env.h"
#include "../Buffer.h"
#include "../ID_glibrary.h"
#include "../Tools.h"
#include <sys/stat.h>
#include "fcntl.h"
#include "Database.h"
#include <iostream>
#include "../Game/Unit_Model.h"
#include "../Game/Resource_Model.h"
#include "../Game/Production_DB.h"

Database::Database()
{
}

Database::~Database()
{
	std::vector<Unit_Model*>::iterator	it;
	std::vector<Unit_Model*>::iterator	end;
	std::vector<Resource_Model*>::iterator	itR;
	std::vector<Resource_Model*>::iterator	endR;

	it = this->unitDB.begin();
	end = this->unitDB.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
	itR = this->resourceDB.begin();
	endR = this->resourceDB.end();
	while (itR != endR)
	{
		delete *itR;
		++itR;
	}
}

int	Database::Init()
{
	if (Init_UnitDB() == -1)
	{
		std::cout << "Unit DB failed to load." << std::endl;
		return -1;
	}
	if (Init_ResourceDB() == -1)
	{
		std::cout << "Resource DB failed to load." << std::endl;
		return -1;
	}
	if (Init_ProductionDB() == -1)
	{
		std::cout << "Prod DB failed to load." << std::endl;
		return -1;
	}
	return (0);
}

int	Database::Init_UnitDB()
{
	Unit_Model*	um;
	int	fd;
	int	readed = 1;
	char**	tab;
	char*	line;
	Buffer	buf(512, true);

	fd = open("Database/Unit_DB", O_RDONLY);
	if (fd == -1)
	{
		std::cout << "Unit_DB cannot be found." << std::endl;
		return -1;
	}
	readed = buf.read(fd, 512);
	line = buf.getline();
	delete line;
	while (readed != 0 && readed != -1)
	{
		line = buf.getline();
		while (line != NULL)
		{
			std::cout << line << std::endl;
			tab = id_str_to_word_tab_char(line, (char*)"\t");
			um = new Unit_Model(tab);
			if (um == NULL)
				return -1;
			this->unitDB.push_back(um);
			delete line;
			line = buf.getline();
		}
		readed = buf.read(fd, 512);
	}
	std::cout << "Unit_DB succesfully loaded" << std::endl;
	if (close(fd) != 0)
		return -1;
	buf.clear();
	return (0);
}

int	Database::Init_ResourceDB()
{
	Resource_Model*	um;
	int	fd;
	int	readed = 1;
	char**	tab;
	char*	line;
	Buffer	buf(512, true);

	fd = open("Database/Resources_DB", O_RDONLY);
	if (fd == -1)
	{
		std::cout << "Resource_DB cannot be found." << std::endl;
		return -1;
	}
	readed = buf.read(fd, 512);
	line = buf.getline();
	delete line;
	while (readed != 0 && readed != -1)
	{
		line = buf.getline();
		while (line != NULL)
		{
			std::cout << line << std::endl;
			tab = id_str_to_word_tab_char(line, (char*)"\t");
			um = new Resource_Model(tab);
			if (um == NULL)
				return -1;
			this->resourceDB.push_back(um);
			delete line;
			line = buf.getline();
		}
		readed = buf.read(fd, 512);
	}
	std::cout << "Resource_DB succesfully loaded" << std::endl;
	if (close(fd) != 0)
		return -1;
	buf.clear();
	return (0);
}

int	Database::Init_ProductionDB()
{
	Production_DB*	um;
	int	fd;
	int	readed = 1;
	char**	tab;
	char*	line;
	Buffer	buf(512, true);

	fd = open("Database/Production_DB", O_RDONLY);
	if (fd == -1)
	{
		std::cout << "Production_DB cannot be found." << std::endl;
		return -1;
	}
	readed = buf.read(fd, 512);
	line = buf.getline();
	delete line;
	while (readed != 0 && readed != -1)
	{
		line = buf.getline();
		while (line != NULL)
		{
			std::cout << line << std::endl;
			tab = id_str_to_word_tab_char(line, (char*)"\t");
			um = new Production_DB(tab);
			if (um == NULL)
				return -1;
			this->prodDB.push_back(um);
			delete line;
			line = buf.getline();
		}
		readed = buf.read(fd, 512);
	}
	std::cout << "Production_DB succesfully loaded" << std::endl;
	if (close(fd) != 0)
		return -1;
	buf.clear();
	return (0);
}

Unit_Model*	Database::getUnitModel(int id)
{
	std::vector<Unit_Model*>::iterator	it;

	it = this->unitDB.begin();
	while (it != this->unitDB.end())
	{
		if ((*it)->GetID() == id)
			return ((*it));
		++it;
	}
	std::cout << "Cannot find Unit Model for " << id << std::endl;
	return (0);
}

Resource_Model*	Database::getResourceModel(int id)
{
	std::vector<Resource_Model*>::iterator	it;

	it = this->resourceDB.begin();
	while (it != this->resourceDB.end())
	{
		if ((*it)->GetID() == id)
			return ((*it));
		++it;
	}
	std::cout << "Cannot find Resource Model for " << id << std::endl;
	return (0);
}

std::vector<Production_DB*>	Database::getProductionList(int id)
{
	std::vector<Production_DB*>		ret;
	std::vector<Production_DB*>::iterator	it;
	int	i = 0;

	it = this->prodDB.begin();
	while (it != this->prodDB.end())
	{
		if ((*it)->builderID == id)
		{
			ret.push_back((*it));
			++i;
		}
		++it;
	}
	if (i > 0)
		return (ret);
	std::cout << "Cannot find Production Model for " << id << std::endl;
	return (ret);
}
