#ifndef __STATE_H__
#define __STATE_H__

#include <vector>
#include <stdint.h>

class	State_Machine;

class	State
{
	friend class	State_Machine;
 public:
	State(uint32_t);
	~State();

	virtual void	Update(void) = 0;
	virtual void	OnEnter(void);
	virtual void	OnLeave(void);
	inline uint32_t	GetID(void) const
	{
		return this->_ID;
	}

 protected:
	State_Machine*	_motherMachine;
	uint32_t	_ID;
};

#endif
