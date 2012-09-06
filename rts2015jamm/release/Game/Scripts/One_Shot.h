#ifndef	__ONE_SHOT_H__
#define	__ONE_SHOT_H__

#include "Script_Model.hpp"
#include "../../ID_glibrary.h"

class	Bullet_Hit_Model;
class	Unit;

class	One_Shot : public Script_Model
{
	public:
				One_Shot();
				~One_Shot();
		virtual void	CallPreview(Unit*);
		virtual int	ApplyEventPreview(ID::Event*, Unit*);
		virtual void	UnitUpdatePreview(Unit* u);
		virtual void	CancelPreview(void);

	public:
		Bullet_Hit_Model*	bhm;

	private:
		ID::GCircle	__preview;
};

#endif
