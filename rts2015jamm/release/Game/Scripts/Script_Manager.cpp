#include "Script_Manager.h"
#include "One_Shot.h"
#include "Gather.h"

Script_Manager::Script_Manager()
{
}

Script_Manager::~Script_Manager()
{
	std::vector<Script_Model*>::iterator	it;
	std::vector<Script_Model*>::iterator	end;

	it = this->__models.begin();
	end = this->__models.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
}

int	Script_Manager::Init(void)
{
	Gather*		gather;
	One_Shot*	OS;

	this->__models.resize(3);
	this->__models[0] = NULL;

	gather = new Gather();
	if (gather == NULL)
		return -1;
	this->__models[1] = gather;

	OS = new One_Shot();
	if (OS == NULL)
		return -1;
	this->__models[2] = OS;
	return 0;
}

Script_Model*	Script_Manager::GetModel(SCRIPT_ID ID)
{
	return this->__models[ID];
}

