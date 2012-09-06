#include "Credit_Screen.h"
#include "../ID_glibrary.h"
#include "../Menu/Nicks_Menu.h"
#include "../Constantes.h"

Credit_Screen::Credit_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT)
{
	this->SetName("Credit_Screen");

	this->nicksMenu = new Nicks_Menu();
	if (this->nicksMenu == NULL)
		throw 78;
	this->focusMenu = this->nicksMenu;
	this->AddChild(this->nicksMenu);
}

Credit_Screen::~Credit_Screen()
{
}

void	Credit_Screen::ApplyEvent(ID::Event* e)
{
	if (this->focusMenu != NULL)
		this->focusMenu->context->ApplyEvent(e);
}

void	Credit_Screen::Refresh()
{
	if (this->focusMenu != NULL)
		this->focusMenu->context->Update();
}

void	Credit_Screen::Enter()
{
	std::cout << "Entering CreditScreen\n";
}

void	Credit_Screen::Leave()
{
	std::cout << "Leaving CreditScreen\n";
}
