#ifndef	__ACTION_HPP__
#define	__ACTION_HPP__

#include <stdint.h>

class	Action_Model;
class	Action
{
	public:
		Action(Action_Model* m) : _model(m),
					  _firstTime(true),
					  _tick(0)
		{
		}
		~Action()
		{
		}
		virtual int	Apply(uint16_t x, uint16_t y,
				      Unit* data, uint32_t tick)
		{
			if (this->_firstTime == true)
			{
				this->_tick = tick;
				this->_model->Apply(x, y, data, tick);
				this->_firstTime = false;
				return 0;
			}
			if (tick - this->_tick == this->_model->GetDuration())
			{
				if (this->_model->GetRedundant() == RDDT_TRUE)
					return 2;
				return 1;
			}
			return 0;
		}

	protected:
		Action_Model*	_model;
		bool		_firstTime;
		uint32_t	_tick;
};

#endif
