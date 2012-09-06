#ifndef	__GATHER_H__
#define	__GATHER_H__

#include "Script_Model.hpp"
#include "../../ID_glibrary.h"

class	Gather_Model;

class	Gather : public Script_Model
{
	public:
		Gather();
		~Gather();
	//	virtual int	Apply(uint16_t x, uint16_t y, void* data);

	public:
		Gather_Model*	rm;
};

#endif
