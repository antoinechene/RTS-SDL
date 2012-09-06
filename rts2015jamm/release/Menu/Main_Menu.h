#ifndef	__MAIN_MENU_H__
#define	__MAIN_MENU_H__

#include "Abstract_Menu.h"

class	Main_Menu : public Abstract_Menu
{
	public:
		Main_Menu();
		~Main_Menu();

		char		_welcomeTitle[30];
		ID::Label	welcomeTitle;
		ID::Label	network;
		ID::Label	option;
		ID::Label	credit;
		ID::Label	exit;
};

#endif
