#ifndef	__ACTION_MODEL_H__
#define	__ACTION_MODEL_H__

#include <stdint.h>

enum
{
	INFINITE	= -1
};

enum	ACTION_PROPERTIES
{
	AP_NONE		= 0,
	AP_END		= 1,
	AP_REDUNDANT	= 2
};

enum	Bool_Redundant
{
	RDDT_TRUE,
	RDDT_FALSE
};

enum	Bool_Persistance
{
	PRST_TRUE,
	PRST_FALSE
};

class	Unit;
class	Script;

class	Action
{
	protected:
				Action();

	public:
				Action(const Action&);
				~Action();
		virtual Action*	Clone(void) = 0;
		virtual void	Init(void);
		virtual int	Apply(Script* s);

		inline void	SetRelX(float X)
		{
			this->_relX = X;
		}
		inline void	SetRelY(float Y)
		{
			this->_relY = Y;
		}
		inline void	SetRadius(float r)
		{
			this->_radius = r;
		}
		inline void	SetDuration(uint32_t drt)
		{
			this->_duration = drt;
		}
		inline void	SetMaxRepeat(uint32_t cr)
		{
			this->_maxRepeat = cr;
		}
		inline void	SetRedundant(Bool_Redundant r)
		{
			this->_redundant = r;
		}
		inline void	SetPersistance(Bool_Persistance p)
		{
			this->_persistance = p;
		}

		inline float	GetRelX(void)
		{
			return this->_relX;
		}
		inline float	GetRelY(void)
		{
			return this->_relY;
		}
		inline float	GetRadius(void)
		{
			return this->_radius;
		}
		inline int32_t	GetDuration(void)
		{
			return this->_duration;
		}
		inline int32_t	GetMaxRepeat(void)
		{
			return this->_maxRepeat;
		}
		inline Bool_Redundant	GetRedundant(void)
		{
			return this->_redundant;
		}
		inline Bool_Persistance	GetPersistance(void)
		{
			return this->_persistance;
		}
		inline Action*	GetParent(void)
		{
			return this->_parent;
		}

	protected:
		virtual int	_Apply(Script*) = 0;

	protected:
		// Self properties
		//sprite;
		float			_relX;
		float			_relY;
		float			_radius; // case
		int32_t			_duration; // tick
		int32_t			_maxRepeat; // Nombre max de repetition, -1 pour infini, ne marche que s'il est _redundant
		Bool_Redundant		_redundant; // Restart the action (Add to the queue until end of script)
		Bool_Persistance	_persistance; // Continue to apply even if the caster is dead

		// Working properties
		int32_t			_repeat;
		bool			_firstTime;
		uint32_t		_tick;
		Action*			_parent;
};

#endif
