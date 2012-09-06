#ifndef	__JOIN_SCREEN_H__
#define	__JOIN_SCREEN_H__

#include "../ID_glibrary.h"
#include "Abstract_Screen.hpp"

class	Join_Screen : public Abstract_Screen
{
	public:
				Join_Screen();
				~Join_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual void	Enter();
		virtual void	Leave();

	public:
		ID::Label		joinTitle;
		ID::Textbox		inputIP;
		ID::Button_Label	joinGame;
		ID::Button_Label	back;
};

#endif
