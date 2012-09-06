#include "Network_Menu.h"
#include "Main_Menu.h"
#include "../Screens/Abstract_Screen.hpp"
#include "../Screens/Main_Screen.h"
#include "../Screens/Host_Screen.h"
#include "../Screens/Join_Screen.h"
#include "../Env.h"
#include "../Constantes.h"
#include "Context_Line.hpp"

void	__Network_Menu__HostCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "Host" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env*	env;
			env = Env::GetInstance();
			env->SwitchScreen(env->hostScreen);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
		{
			Env*	env;
			env = Env::GetInstance();
			env->mainScreen->focusMenu = env->mainScreen->mainMenu;
			env->mainScreen->networkMenu->SetOnScreen(false);
		}
	}
}

void	__Network_Menu__JoinCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "Join" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env*	env;
			env = Env::GetInstance();
			env->SwitchScreen(env->joinScreen);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
		{
			Env*	env;
			env = Env::GetInstance();
			env->mainScreen->focusMenu = env->mainScreen->mainMenu;
			env->mainScreen->networkMenu->SetOnScreen(false);
		}
	}
}

void	__Network_Menu__CbClickDown(ID::Control*, ID::Event*, void* d)
{
	Env::GetInstance()->mainScreen->networkMenu->context->focus = (int)d;
}

ID::Color	Network_Menu_Color	= ID::Color(235, 235, 0, 255);

Network_Menu::Network_Menu() : Abstract_Menu(UI_WIDTH, UI_HEIGHT),
			       network(DEFAULT_FONT_PATH,
					40, (char*)"Network",
					&Network_Menu_Color),
			       host(DEFAULT_FONT_PATH,
				    40, (char*)"Host",
				    &Network_Menu_Color),
			       join(DEFAULT_FONT_PATH,
				    40, (char*)"Join",
				    &Network_Menu_Color)
{
	this->SetName("Network_Menu");

	this->context = Menu_Manager<ID::Label>::GetInstance()->AddContext(3);
	if (this->context == NULL)
		throw 180;
	this->context->focus = 1;
	this->updater = new Line_Updater<ID::Label>();
	if (this->updater == NULL)
		throw 181;
	this->context->SetMenu(0, &(this->network));
	this->context->SetMenu(1, &(this->host));
	this->context->SetMenu(2, &(this->join));
	this->context->SetUpdater(this->updater);

	this->network.SetName((char*)"Network_Menu_Network");
	this->AddChild(&(this->network));

	this->host.SetName((char*)"Network_Menu_Host");
	this->host.SetCbClickDown(__Network_Menu__CbClickDown, (void*)1);
	this->host.SetCbUserEvent(__Network_Menu__HostCbUserEvent, 0);
	this->AddChild(&(this->host));

	this->join.SetName((char*)"Network_Menu_Join");
	this->join.SetCbClickDown(__Network_Menu__CbClickDown, (void*)2);
	this->join.SetCbUserEvent(__Network_Menu__JoinCbUserEvent, 0);
	this->AddChild(&(this->join));
}

Network_Menu::~Network_Menu()
{
	delete this->context;
	delete this->updater;
}

