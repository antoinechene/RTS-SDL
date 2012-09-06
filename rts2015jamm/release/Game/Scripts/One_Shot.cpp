#include "One_Shot.h"
#include "Actions/Bullet_Hit_Model.h"
#include "../Cam.h"
#include "../Game.h"
#include "../Unit.h"
#include "../../ID_glibrary_Extension/Map_Drawable.h"
#include "../../Constantes.h"

ID::Color	__One_Shot_rangeColor = ID::Color(0xFF, 0xFF, 0, 80);

One_Shot::One_Shot() : Script_Model((char*)"One Shot",
				    (char*)"Snipe at high speed the head of the target with a bullet.",
				    0, 100,
				    72, 0, 24,
				    CCBL_FALSE,
				    TAS_FALSE,
				    7, 0,
				    true),
		       __preview(7 * CASE_SIDE_PIXELS, true, (ID::Color*)&__One_Shot_rangeColor)

{
	this->bhm = new Bullet_Hit_Model();
	if (this->bhm == NULL)
		throw 845;
	this->_SetActions(1, this->bhm);
}

One_Shot::~One_Shot()
{
	delete this->bhm;
}

void	One_Shot::CallPreview(Unit* u)
{
	Cam*	c;
	double	x;
	double	y;

	c = Cam::GetInstance();
	u->GetPositionXY(&x, &y);
	this->__preview.SetPos(x * CASE_SIDE_PIXELS - c->GetOffsetX(),
			       y * CASE_SIDE_PIXELS - c->GetOffsetY());
	c->GetMapDrawable()->AddGround(&(this->__preview), ON_BACKGROUND);
}

int	One_Shot::ApplyEventPreview(ID::Event* e, Unit* u)
{
	if (e->type == ID::EVENT_CLICK_DOWN && u != NULL)
		//#TODO Add check range, unit's team, 's player, etc...
		return 1;
	return 0;
}

void	One_Shot::UnitUpdatePreview(Unit* u)
{
	Cam*	c;
	double	x;
	double	y;

	c = Cam::GetInstance();
	u->GetPositionXY(&x, &y);
	this->__preview.SetPos(x * CASE_SIDE_PIXELS - c->GetOffsetX(),
			       y * CASE_SIDE_PIXELS - c->GetOffsetY());
}

void	One_Shot::CancelPreview(void)
{
	Cam::GetInstance()->GetMapDrawable()->DelGround(&(this->__preview), ON_BACKGROUND);
}
