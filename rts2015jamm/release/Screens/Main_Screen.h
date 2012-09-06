#ifndef	__MAIN_SCREEN_H__
#define	__MAIN_SCREEN_H__

#include "Abstract_Screen.hpp"

class	Abstract_Menu;
class	Main_Menu;
class	Network_Menu;
class	Main_Screen : public Abstract_Screen
{
	public:
				Main_Screen();
				~Main_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual	void	Enter();
		virtual	void	Leave();

	public:
		Abstract_Menu*		focusMenu;
		Main_Menu*		mainMenu;
		Network_Menu*		networkMenu;
};

#endif
