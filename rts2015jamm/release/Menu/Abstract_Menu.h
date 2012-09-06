#ifndef	__ABSTRACT_MENU_H__
#define	__ABSTRACT_MENU_H__

#include "../ID_glibrary.h"
#include "../Constantes.h"
#include "Menu_Manager.hpp"

class	Abstract_Menu : public ID::Drawable
{
	public:
		Abstract_Menu(uint16_t w, uint16_t h) : Drawable(w, h, UI_FLAGS,
								  UI_RMASK, UI_GMASK,
								  UI_BMASK, UI_AMASK)
		{
		}
		Menu_Context<ID::Label>*	context;
		Context_Updater<ID::Label>*	updater;
};

#endif
