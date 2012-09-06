#ifndef	__NETWORK_MENU_H__
#define	__NETWORK_MENU_H__

#include "Abstract_Menu.h"

class	Network_Menu : public Abstract_Menu
{
	public:
		Network_Menu();
		~Network_Menu();

		ID::Label	network;
		ID::Label	host;
		ID::Label	join;
};

#endif
