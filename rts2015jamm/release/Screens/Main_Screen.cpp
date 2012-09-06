#include "Main_Screen.h"
#include "../ID_glibrary.h"
#include "../Menu/Main_Menu.h"
#include "../Menu/Network_Menu.h"
#include "../Constantes.h"

//#include <sys/time.h>

Main_Screen::Main_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT)
{
	this->SetName("Main_Screen");

	this->mainMenu = new Main_Menu();
	if (this->mainMenu == NULL)
		throw 40;
	this->focusMenu = this->mainMenu;
	this->AddChild(this->mainMenu);

	this->networkMenu = new Network_Menu();
	if (this->networkMenu == NULL)
		throw 41;
	this->networkMenu->SetOnScreen(false);
	this->AddChild(this->networkMenu);
}

Main_Screen::~Main_Screen()
{
}

void	Main_Screen::ApplyEvent(ID::Event* e)
{
	if (this->focusMenu != NULL)
		this->focusMenu->context->ApplyEvent(e);
}

void	Main_Screen::Refresh()
{
/*
	// BENCHMARK POUR LE MENU
	struct timeval	tv;
	static uint32_t	last = 0;
*/
	if (this->focusMenu != NULL)
	{
//		gettimeofday(&tv, NULL);
		this->focusMenu->context->Update();
/*		std::cout << "Time passed="
			<< (tv.tv_sec * 1000000 + tv.tv_usec) - last << std::endl;
		last = tv.tv_sec * 1000000 + tv.tv_usec;
*/	}
}

void	Main_Screen::Enter()
{
	std::cout << "Entering MainScreen\n";
}

void	Main_Screen::Leave()
{
	std::cout << "Leaving MainScreen\n";
}
