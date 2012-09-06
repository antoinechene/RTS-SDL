#ifndef	__ATTACK_HANDLER_H__
#define	__ATTACK_HANDLER_H__

class	Unit;

class	Attack_Handler
{
	public:
			Attack_Handler(Unit*);
			~Attack_Handler();
		virtual bool	CanAttack(Unit*) = 0;
		virtual int	Attack(Unit*) = 0;

	protected:
		Unit*	_unit;
};

#endif
