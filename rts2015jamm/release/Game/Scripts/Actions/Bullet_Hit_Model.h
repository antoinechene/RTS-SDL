#ifndef	__BULLET_HIT_MODEL_H__
#define __BULLET_HIT_MODEL_H__

#include "Action.h"

class	Bullet_Hit_Model : public Action
{
	public:
		Bullet_Hit_Model();
		~Bullet_Hit_Model();
		virtual Action*	Clone(void);

	protected:
		virtual int	_Apply(Script*);

	protected:
		int		_HPdamage;
};

#endif
