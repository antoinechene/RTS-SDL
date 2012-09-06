#ifndef	__GROUND_ATTACK_HANDLER_H__
#define	__GROUND_ATTACK_HANDLER_H__

#include "Attack_Handler.h"
#include <stdint.h>

class	Node;

class	Ground_Attack_Handler : public Attack_Handler
{
	public:
				Ground_Attack_Handler(Unit*);
				~Ground_Attack_Handler();
		virtual bool	CanAttack(Unit*);
		virtual int	Attack(Unit*);

	protected:
		uint32_t	_lastAttack;
};

#endif
