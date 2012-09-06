#include "Gather.h"
#include "Actions/Gather_Model.h"

Gather::Gather() : Script_Model((char*)"Gather",
				(char*)"Earn resources.",
				0, 0,
				0, 0, 0,
				CCBL_FALSE,
				TAS_TRUE,
				0, 0,
				false)
{
	this->rm = new Gather_Model(1, 10, 2);
	if (this->rm == NULL)
		throw 845;
	this->rm->SetRedundant(RDDT_TRUE);
	this->rm->SetMaxRepeat(INFINITE);
	this->_SetActions(1, this->rm);
}

Gather::~Gather()
{
	delete this->rm;
}

