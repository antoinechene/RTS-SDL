#ifndef	__ABSTRACT_SCREEN_H__
#define	__ABSTRACT_SCREEN_H__

#include "../ID_glibrary.h"
#include "../Constantes.h"

class	Abstract_Screen : public ID::Drawable
{
	public:
		Abstract_Screen(uint16_t w, uint16_t h) : Drawable(w, h, UI_FLAGS,
								  UI_RMASK, UI_GMASK,
								  UI_BMASK, UI_AMASK)
		{
		}
		virtual void	ApplyEvent(ID::Event*) = 0;
		virtual void	Refresh() = 0;
		virtual void	Enter() = 0;
		virtual void	Leave() = 0;
};

#endif
