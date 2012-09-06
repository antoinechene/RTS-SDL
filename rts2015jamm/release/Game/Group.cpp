#include "Group.h"
#include "Unit.h"

Group::Group()
{
}

Group::~Group()
{
}

void	Group::AddUnit(Unit* u)
{
	if (u->GetGroups()[this->id] == NULL)
	{
		u->GetGroups()[this->id] = this;
		this->_units.push_back(u);
	}
}

void	Group::DelUnit(Unit* u)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;

	it = this->_units.begin();
	end = this->_units.end();
	while (it != end)
	{
		if (*it == u)
		{
			(*it)->GetGroups()[this->id] = NULL;
			this->_units.erase(it);
			return ;
		}
		++it;
	}
}

void	Group::ClearUnits(void)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;

	it = this->_units.begin();
	end = this->_units.end();
	while (it != end)
	{
		(*it)->GetGroups()[this->id] = NULL;
		++it;
	}
	this->_units.clear();
}

std::list<Unit*>*	Group::GetUnits(void)
{
	return &(this->_units);
}
