#ifndef	__GATHER_MODEL_H__
#define __GATHER_MODEL_H__

#include "Action.h"

class	Resource_Model;

class	Gather_Model : public Action
{
	public:
		Gather_Model(int ID, int delta, float interval);
		Gather_Model(const Gather_Model&);
		~Gather_Model();
		virtual Action*	Clone(void);

	protected:
		virtual int	_Apply(Script*);

	protected:
		int		_delta;
		int		_resourceID;
		Resource_Model*	_rm;
		uint32_t	_tick;
		uint32_t	_maxTick;
};

#endif
