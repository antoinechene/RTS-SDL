#ifndef	__OPTION_SCREEN_H__
#define	__OPTION_SCREEN_H__

#include "../ID_glibrary.h"
#include "Abstract_Screen.hpp"

class	Option_Screen;

void	__keyHR_CbKeyDown(ID::Control*, ID::Event*, void*);
void	check_shortcut(ID::Event*, Option_Screen* os);
typedef	void (*callback)(void);

class	Shortcut_Cmd
{
	friend void	__keyHR_CbKeyDown(ID::Control*, ID::Event*, void*);
	public:
		Shortcut_Cmd(char* name, char* defaultKey, int x, int y);
		~Shortcut_Cmd()
		{
		}

		void	SetCb(callback);
	public:
		ID::Label	name;
		ID::Key		key;
		ID::Textbox	keyHR; // Human Readable

		callback	_cbShortcut;
};

class	Option_Screen : public Abstract_Screen
{
	public:
				Option_Screen();
				~Option_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual void	Enter();
		virtual void	Leave();

	public:
		ID::Label		optionTitle;
		ID::Label		nickname;
		ID::Textbox		inputNickname;
		Shortcut_Cmd		talk;
		Shortcut_Cmd		teamTalk;
		Shortcut_Cmd		cameraUp;
		Shortcut_Cmd		cameraDown;
		Shortcut_Cmd		cameraLeft;
		Shortcut_Cmd		cameraRight;
		Shortcut_Cmd		move;
		Shortcut_Cmd		attack;
		Shortcut_Cmd		patrol;
		Shortcut_Cmd		hold;
		Shortcut_Cmd		assist;
		Shortcut_Cmd		stop;
		Shortcut_Cmd		hidleWorker;
		ID::Button_Label	toggleFullScreen;
		ID::Button_Label	validateOptions;
		ID::Button_Label	back;
};
#endif
