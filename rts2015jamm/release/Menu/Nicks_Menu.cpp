#include "Nicks_Menu.h"
#include "Context_Arc.hpp"
#include "../Env.h"
#include "../Buffer.h"
#include "../Constantes.h"
#include "../Screens/Credit_Screen.h"

void	__Nicks_Menu__AEBCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "AEB" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
	/*	if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->optionScreen);
		}
		else*/ if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Nicks_Menu__MCCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "MC" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
	/*	if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->optionScreen);
		}
		else*/ if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}


void	__Nicks_Menu__MNCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "MN" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
	/*	if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->optionScreen);
		}
		else*/ if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}


void	__Nicks_Menu__PMCbUserEvent(ID::Control*, ID::Event* e, void*)
{
	std::cout << "PM" << std::endl;
	if (e->type == ID::EVENT_KEY_DOWN)
	{
	/*	if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->optionScreen);
		}
		else*/ if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			Env::GetInstance()->alive = false;
	}
}

void	__Nicks_Menu__CbClickDown(ID::Control*, ID::Event*, void* d)
{
	Env::GetInstance()->creditScreen->focusMenu->context->focus = (int)d;
}

ID::Color	Nicks_Menu_Color = ID::Color(255, 255, 0, 255);

Nicks_Menu::Nicks_Menu() : Abstract_Menu(UI_WIDTH, UI_HEIGHT),
			   AEschenBrenner(DEFAULT_FONT_PATH,
				  50, (char*)"A. Eschen Brenner",
				  &Nicks_Menu_Color),
			   MCeneda(DEFAULT_FONT_PATH,
				  50, (char*)"M. Ceneda",
				  &Nicks_Menu_Color),
			   MNguyen(DEFAULT_FONT_PATH,
				  50, (char*)"M. Nguyen",
				  &Nicks_Menu_Color),
			   PMartin(DEFAULT_FONT_PATH,
				  50, (char*)"P. Martinez",
				  &Nicks_Menu_Color)
{
	this->SetName("Nicks_Menu");

	this->context = Menu_Manager<ID::Label>::GetInstance()->AddContext(4);
	if (this->context == NULL)
		throw 180;
	this->updater = new Arc_Updater<ID::Label>();
	if (this->updater == NULL)
		throw 181;
	this->context->SetMenu(0, &(this->AEschenBrenner));
	this->context->SetMenu(1, &(this->MCeneda));
	this->context->SetMenu(2, &(this->MNguyen));
	this->context->SetMenu(3, &(this->PMartin));
	this->context->SetUpdater(this->updater);

	this->AEschenBrenner.SetName((char*)"Nicks_Menu_AEB");
	this->AEschenBrenner.SetCbClickDown(__Nicks_Menu__CbClickDown, (void*)0);
	this->AEschenBrenner.SetCbUserEvent(__Nicks_Menu__AEBCbUserEvent, 0);
	this->AddChild(&(this->AEschenBrenner));

	this->MCeneda.SetName((char*)"Nicks_Menu_MC");
	this->MCeneda.SetCbClickDown(__Nicks_Menu__CbClickDown, (void*)1);
	this->MCeneda.SetCbUserEvent(__Nicks_Menu__MCCbUserEvent, 0);
	this->AddChild(&(this->MCeneda));

	this->MNguyen.SetName((char*)"Nicks_Menu_MN");
	this->MNguyen.SetCbClickDown(__Nicks_Menu__CbClickDown, (void*)2);
	this->MNguyen.SetCbUserEvent(__Nicks_Menu__MNCbUserEvent, 0);
	this->AddChild(&(this->MNguyen));

	this->PMartin.SetName((char*)"Nicks_Menu_PM");
	this->PMartin.SetCbClickDown(__Nicks_Menu__CbClickDown, (void*)3);
	this->PMartin.SetCbUserEvent(__Nicks_Menu__PMCbUserEvent, 0);
	this->AddChild(&(this->PMartin));
}

Nicks_Menu::~Nicks_Menu()
{
	delete this->context;
	delete this->updater;
}

