#ifndef	__HOST_SCREEN_H__
#define	__HOST_SCREEN_H__

#include "../ID_glibrary.h"
#include "Abstract_Screen.hpp"

class	Host_Screen : public Abstract_Screen
{
	public:
			Host_Screen();
			~Host_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual void	Enter();
		virtual void	Leave();

	public:
		ID::Label		hostTitle;
		ID::Textbox		inputMap;
		ID::Button_Label	hostGame;
		ID::Button_Label	back;
};

#endif
