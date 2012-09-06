#include "Action.h"
#include "../Script.hpp"

Action::Action() : _relX(0),
		   _relY(0),
		   _radius(0),
		   _duration(0),
		   _maxRepeat(0),
		   _redundant(RDDT_FALSE),
		   _persistance(PRST_TRUE),
		   _firstTime(false),
		   _tick(0),
		   _parent(NULL)
{
}

Action::Action(const Action& a)
{
	std::cout << "CPY Const Action";
	this->_relX = a._relX;
	this->_relY = a._relY;
	this->_radius = a._radius;
	this->_duration = a._duration;
	this->_maxRepeat = a._maxRepeat;
	this->_redundant = a._redundant;
	this->_persistance = a._persistance;
	this->_repeat = a._repeat;
	this->_firstTime = a._firstTime;
	this->_tick = a._tick;
}

Action::~Action()
{
}

void	Action::Init(void)
{
	this->_repeat = 0;
	this->_firstTime = true;
}

int	Action::Apply(Script* s)
{
	int	ret;

	ret = AP_NONE;
	if (this->_firstTime == true)
	{
		this->_tick = s->GetTick();
		this->_firstTime = false;
//		return ret;
	}
		this->_Apply(s);
	if ((int32_t)(s->GetTick() - this->_tick) == this->GetDuration())
	{
		this->_firstTime = true;
		ret |= AP_END;
		if (this->GetRedundant() == RDDT_TRUE)
		{
			if (this->_repeat < this->_maxRepeat)
				ret |= AP_REDUNDANT;
			++(this->_repeat);
		}
	}
	return ret;
}

