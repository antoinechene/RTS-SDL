#include "Env.h"
#include "ID_glibrary.h"
#include "Screens/Main_Screen.h"
#include "Screens/Host_Screen.h"
#include "Screens/Join_Screen.h"
#include "Screens/Option_Screen.h"
#include "Screens/Credit_Screen.h"
#include "Screens/Lobby_Screen.h"
#include "Screens/Game_Screen.h"

Env::Env() : alive(true),
	     height(UI_HEIGHT),
	     width(UI_WIDTH),
	     focusScreen(NULL),
	     hostScreen(NULL),
	     joinScreen(NULL),
	     optionScreen(NULL),
	     lobbyScreen(NULL)
{
	strcpy(this->nickname, "Noname");
}

Env::~Env()
{
	if (pthread_mutex_destroy(&(this->dataMutex)) != 0)
		std::cerr << "Error Env: pthread_mutex_destroy failed\n";
	if (pthread_mutex_destroy(&(this->displayMutex)) != 0)
		std::cerr << "Error Env: pthread_mutex_destroy failed\n";

	delete this->mainScreen;
	delete this->hostScreen;
	delete this->joinScreen;
	delete this->optionScreen;
	delete this->creditScreen;
	delete this->lobbyScreen;
	delete this->gameScreen;
}

int	Env::Init(ID::Window* w)
{
	this->window = w;
	if (pthread_mutex_init(&(this->dataMutex), NULL) != 0)
	{
		std::cerr << "Error init data mutex" << std::endl;
		throw 20;
	}
	if (pthread_mutex_init(&(this->displayMutex), NULL) != 0)
	{
		std::cerr << "Error init display mutex" << std::endl;
		throw 21;
	}

	this->mainScreen = new Main_Screen();
	if (this->mainScreen == NULL)
		throw 24;
	this->mainScreen->SetOnScreen(true);
	w->AddChild(this->mainScreen);

	this->hostScreen = new Host_Screen();
	if (this->hostScreen == NULL)
		throw 25;
	this->hostScreen->SetOnScreen(false);
	w->AddChild(this->hostScreen);

	this->joinScreen = new Join_Screen();
	if (this->joinScreen == NULL)
		throw 26;
	this->joinScreen->SetOnScreen(false);
	w->AddChild(this->joinScreen);

	this->optionScreen = new Option_Screen();
	if (this->optionScreen == NULL)
		throw 27;
	this->optionScreen->SetOnScreen(false);
	w->AddChild(this->optionScreen);

	this->creditScreen = new Credit_Screen();
	if (this->creditScreen == NULL)
		throw 28;
	this->creditScreen->SetOnScreen(false);
	w->AddChild(this->creditScreen);

	this->lobbyScreen = new Lobby_Screen();
	if (this->lobbyScreen == NULL)
		throw 29;
	this->lobbyScreen->SetOnScreen(false);
	w->AddChild(this->lobbyScreen);

	this->gameScreen = new Game_Screen();
	if (this->gameScreen == NULL)
		throw 30;
	this->gameScreen->SetOnScreen(false);
	w->AddChild(this->gameScreen);

	this->focusScreen = this->mainScreen;

	return 0;
}

void	Env::SwitchScreen(Abstract_Screen* as)
{
	this->focusScreen->SetOnScreen(false);
	this->focusScreen->Leave();
	this->focusScreen = as;
	this->focusScreen->SetOnScreen(true);
	this->focusScreen->Enter();
}
