#ifndef	__CREDIT_SCREEN_H__
#define	__CREDIT_SCREEN_H__

#include "Abstract_Screen.hpp"

class	Abstract_Menu;
class	Nicks_Menu;

class	Credit_Screen : public Abstract_Screen
{
	public:
				Credit_Screen();
				~Credit_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual	void	Enter();
		virtual	void	Leave();

	public:
		Abstract_Menu*		focusMenu;
		Nicks_Menu*		nicksMenu;
};

#endif
