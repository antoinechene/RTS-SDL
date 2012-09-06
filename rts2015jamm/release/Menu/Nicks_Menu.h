#ifndef	__NICKS_MENU_H__
#define	__NICKS_MENU_H__

#include "Abstract_Menu.h"

class	Nicks_Menu : public Abstract_Menu
{
	public:
		Nicks_Menu();
		~Nicks_Menu();

		ID::Label	AEschenBrenner;
		ID::Label	MCeneda;
		ID::Label	MNguyen;
		ID::Label	PMartin;
};

#endif
