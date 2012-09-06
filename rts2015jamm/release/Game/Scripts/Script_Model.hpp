#ifndef	__SCRIPT_MODEL_HPP__
#define	__SCRIPT_MODEL_HPP__

#include "Actions/Action.h"
#include "../../ID_glibrary.h"
#include <string.h>
#include <cstdarg>
#include <stdint.h>

// Case = 32 pixels default
#define	NAME_LENGTH		20
#define	DESCRIPTION_LENGTH	512

enum	Bool_Cancelable
{
	CCBL_TRUE,
	CCBL_FALSE
};

enum	Bool_TriggeredAtStart
{
	TAS_TRUE,
	TAS_FALSE
};

class	Unit;
class	Action;

class	Script_Model
{
	public:
		Script_Model(char* n,
			     char* d,
			     int32_t costHP,
			     int32_t costMP,
			     uint32_t CD,
			     uint32_t duration,
			     uint32_t CT,
			     Bool_Cancelable CCBL,
			     Bool_TriggeredAtStart TAS,
			     double range,
			     double radius,
			     bool preview
			     ) : _costHP(costHP),
				 _costMP(costMP),
				 _cooldown(CD),
				 _duration(duration),
				 _castTime(CT),
				 _cancelable(CCBL),
				 _triggeredAtStart(TAS),
				 _range(range),
				 _radius(radius),
				 _preview(preview)
		{
			int	i;

			i = 0;
			while (i < NAME_LENGTH && n[i] != 0)
			{
				this->_name[i] = n[i];
				++i;
			}
			this->_name[NAME_LENGTH] = 0;
			i = 0;
			while (i < DESCRIPTION_LENGTH && d[i] != 0)
			{
				this->_description[i] = d[i];
				++i;
			}
			this->_description[DESCRIPTION_LENGTH] = 0;
		}
		~Script_Model()
		{
			delete[] this->_name;
			delete[] this->_actions;
		}

		virtual void	CallPreview(Unit*)
		{
		}
		virtual int	ApplyEventPreview(ID::Event*, Unit*)
		{
			return 0;
		}
		virtual void	UnitUpdatePreview(Unit*)
		{
		}
		virtual void	CancelPreview(void)
		{
		}

//		virtual int	Apply(uint16_t x, uint16_t y, void* data) = 0;

		inline void	SetCostHP(int32_t HP)
		{
			this->_costHP = HP;
		}
		inline void	SetCostMP(int32_t MP)
		{
			this->_costMP = MP;
		}
		inline void	SetCooldown(int32_t CD)
		{
			this->_cooldown = CD;
		}
		inline void	SetCastTime(int32_t ct)
		{
			this->_castTime = ct;
		}
		inline void	SetCancelable(Bool_Cancelable c)
		{
			this->_cancelable = c;
		}
		inline void	SetTriggeredAtStart(Bool_TriggeredAtStart c)
		{
			this->_triggeredAtStart = c;
		}
		inline void	SetRange(int32_t r)
		{
			this->_range = r;
		}

		inline char*	GetName(void)
		{
			return this->_name;
		}
		inline char*	GetDescription(void)
		{
			return this->_description;
		}
		inline int32_t	GetCostHP(void)
		{
			return this->_costHP;
		}
		inline int32_t	GetCostMP(void)
		{
			return this->_costMP;
		}
		inline uint32_t	GetCooldown(void)
		{
			return this->_cooldown;
		}
		inline int32_t	GetDuration(void)
		{
			return this->_duration;
		}
		inline uint32_t	GetCastTime(void)
		{
			return this->_castTime;
		}
		inline Bool_Cancelable	GetCancelable(void)
		{
			return this->_cancelable;
		}
		inline Bool_TriggeredAtStart	GetTriggeredAtStart(void)
		{
			return this->_triggeredAtStart;
		}
		inline double	GetRange(void)
		{
			return this->_range;
		}
		inline double	GetRadius(void)
		{
			return this->_radius;
		}
		inline bool	GetPreview(void)
		{
			return this->_preview;
		}

	public:
		Action**	_actions;

	protected:
		int	_SetActions(uint32_t n, ...)
		{
			va_list		l;
			uint32_t	i;

			va_start(l, n);
			this->_actions = new Action*[n + 1];
			if (this->_actions == NULL)
				return -1;
			i = 0;
			while (i < n)
			{
				this->_actions[i] = va_arg(l, Action*);
				++i;
			}
			this->_actions[i] = NULL;
			va_end(l);
			this->__PreCalcul();
			return 0;
		}

	protected:
		char		_name[NAME_LENGTH + 1];
		char		_description[DESCRIPTION_LENGTH + 1];
		int32_t		_costHP;
		int32_t		_costMP;
		uint32_t	_cooldown; // tick
		int32_t		_duration; // tick
		uint32_t	_castTime; // tick
		Bool_Cancelable	_cancelable;
		Bool_TriggeredAtStart	_triggeredAtStart;
		double		_range; // Case
		double		_radius; // Case
		bool		_preview;
		//Picture*	_icon;
		//Animation*	_sprite;

	private:
		void	__PreCalcul(void)
		{
			int	i;

			this->_duration = 0;
			i = 0;
			while (this->_actions[i] != NULL)
			{
				if (this->_actions[i]->GetRedundant() == RDDT_TRUE)
				{
					if (this->_actions[i]->GetMaxRepeat() == INFINITE)
					{
						this->_duration = INFINITE;
						return ;
					}
					this->_duration += this->_actions[i]->GetMaxRepeat() *
							   this->_actions[i]->GetDuration();
				}
				this->_duration += this->_actions[i]->GetDuration();
				++i;
			}
		}
};

#endif
