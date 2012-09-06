#include "Gather_Model.h"
#include "../Script.hpp"
#include "../../Unit.h"
#include "../../Player.h"
#include "../../../Database/Database.h"
#include "../../../Constantes.h"

Gather_Model::Gather_Model(int ID,
			   int d,
			   float i) : _delta(d),
				      _resourceID(ID),
				      _tick(0),
				      _maxTick(i * MAX_TPS)
{
	this->_rm = Database::GetInstance()->getResourceModel(ID);
}

Gather_Model::Gather_Model(const Gather_Model& gm) : Action(gm)
{
	this->_delta = gm._delta;
	this->_resourceID = gm._resourceID;
	this->_rm = gm._rm;
	this->_tick = gm._tick;
	this->_maxTick = gm._maxTick;
}

Gather_Model::~Gather_Model()
{
}

Action*	Gather_Model::Clone(void)
{
	Gather_Model*	c;

	c = new Gather_Model(*this);
	if (c == NULL)
		return NULL;
	return c;
}

int	Gather_Model::_Apply(Script* s)
{
	if (this->_tick == this->_maxTick)
	{
		std::cout << "Gather_Model\n";
		this->_tick = 0;
		s->GetCaster()->GetPlayer()->AddResource(this->_rm, this->_delta);
	}
	else
		++this->_tick;
	return 0;
}

