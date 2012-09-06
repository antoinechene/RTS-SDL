#include "Main_Menu.h"
#include "Network_Menu.h"
#include "Context_Arc.hpp"
#include "../Env.h"
#include "../Buffer.h"
#include "../Constantes.h"
#include "../Screens/Main_Screen.h"
#include "../Screens/Option_Screen.h"
#include "../Screens/Credit_Screen.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

void	__Main_Menu__NetworkCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	Env*	env;

	env = Env::GetInstance();
	std::cout << "Network" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			env->mainScreen->focusMenu = env->mainScreen->networkMenu;
			env->mainScreen->focusMenu->SetOnScreen(true);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Main_Menu__OptionCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "Option" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->optionScreen);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Main_Menu__CreditCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "Credit" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->creditScreen);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Main_Menu__ExitCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "Exit" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER ||
		    e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Main_Menu__CbClickDown(ID::Control*, ID::Event*, void* d)
{
	Env::GetInstance()->mainScreen->focusMenu->context->focus = (int)d;
}

void	init_player_options()
{
	int	fd;
	Env*	e;
	int	readed = 1;
	char*	line;
	int	readedline = 0;
	Buffer	buf(512);

	e = Env::GetInstance();
	if ((fd = open("Resources/options", O_RDONLY)) == -1)
		std::cout << "Can not find the options file, WHAT HAVE YOU DONE?!" << std::endl;
	else
	{
		readed = buf.read(fd, buf.space());
		while (readed != 0 || readed == -1)
		{
			line = buf.getline();
			if (line == 0)
				break ;
			if (readedline == 0)
			{
				std::cout << line << std::endl;
				if (strlen(line) > 0)
					strncpy(e->nickname, line, 20);
				else
					strcpy(e->nickname, "noname");
				delete line;
				break ;
			}
			++readedline;
			delete line;
		}
		close(fd);
	}
	std::cout << "Welcome " << e->nickname << std::endl;
}

char*	get_welcome_message(Main_Menu* mm = NULL)
{
	Env*		env;

	init_player_options();
	env = Env::GetInstance();
	if (mm == NULL)
		mm = env->mainScreen->mainMenu;
	strcpy(mm->_welcomeTitle, "Welcome ");
	strcat(mm->_welcomeTitle, env->nickname);
	return mm->_welcomeTitle;
}

ID::Color	Main_Menu_Color	= ID::Color(255, 255, 0, 255);

Main_Menu::Main_Menu() : Abstract_Menu(UI_WIDTH, UI_HEIGHT),
			 welcomeTitle(DEFAULT_FONT_PATH, 40,
				      get_welcome_message(this),
				      (ID::Color*)&(ID::COLOR_GOLD)),
			 network(DEFAULT_FONT_PATH,
				 50, (char*)"Network",
				 &Main_Menu_Color),
			 option(DEFAULT_FONT_PATH,
				 50, (char*)"Option",
				 &Main_Menu_Color),
			 credit(DEFAULT_FONT_PATH,
				 50, (char*)"Credit",
				 &Main_Menu_Color),
			 exit(DEFAULT_FONT_PATH,
			      50, (char*)"Exit",
			      &Main_Menu_Color)
{
	this->SetName("Main_Menu");

	this->context = Menu_Manager<ID::Label>::GetInstance()->AddContext(4);
	if (this->context == NULL)
		throw 180;
	this->updater = new Arc_Updater<ID::Label>();
	if (this->updater == NULL)
		throw 181;
	this->context->SetMenu(0, &(this->network));
	this->context->SetMenu(1, &(this->option));
	this->context->SetMenu(2, &(this->credit));
	this->context->SetMenu(3, &(this->exit));
	this->context->SetUpdater(this->updater);

	this->welcomeTitle.SetName((char*)"WelcomeTitle");
	this->welcomeTitle.SetPos(15, 15);
	this->AddChild(&(this->welcomeTitle));

	this->network.SetName((char*)"Main_Menu_Network");
	this->network.SetCbClickDown(__Main_Menu__CbClickDown, 0);
	this->network.SetCbUserEvent(__Main_Menu__NetworkCbUserEvent, 0);
	this->AddChild(&(this->network));

	this->option.SetName((char*)"Main_Menu_Option");
	this->option.SetCbClickDown(__Main_Menu__CbClickDown, (void*)1);
	this->option.SetCbUserEvent(__Main_Menu__OptionCbUserEvent, 0);
	this->AddChild(&(this->option));

	this->credit.SetName((char*)"Main_Menu_Credit");
	this->credit.SetCbClickDown(__Main_Menu__CbClickDown, (void*)2);
	this->credit.SetCbUserEvent(__Main_Menu__CreditCbUserEvent, 0);
	this->AddChild(&(this->credit));

	this->exit.SetName((char*)"Main_Menu_Exit");
	this->exit.SetCbClickDown(__Main_Menu__CbClickDown, (void*)3);
	this->exit.SetCbUserEvent(__Main_Menu__ExitCbUserEvent, 0);
	this->AddChild(&(this->exit));
}

Main_Menu::~Main_Menu()
{
	delete this->context;
	delete this->updater;
}

