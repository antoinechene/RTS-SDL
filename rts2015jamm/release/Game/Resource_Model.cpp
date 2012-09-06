#include "Resource_Model.h"
#include <stdlib.h>

Resource_Model::Resource_Model(char** tab)
{
	this->ID = atoi(tab[0]);
	this->Name = tab[1];
}

void	Resource_Model::SetID(int id)
{
	this->ID = id;
}

int	Resource_Model::GetID()
{
	return (this->ID);
}

void	Resource_Model::SetName(char* name)
{
	this->Name = name;
}

char*	Resource_Model::GetName()
{
	return (this->Name);
}
