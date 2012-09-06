#include "Host_Screen.h"
#include "Lobby_Screen.h"
#include "Main_Screen.h"
#include "../Env.h"
#include "../Constantes.h"
#include "../Game/Game.h"
#include "../Game/Player.h"
#include "../Network/Sockets_manager.h"
#include "../Network/Socket.h"
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

void	__Host_Screen__hostGameCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "Host Game\n";
	Sockets_manager*	sm;
	Env*			env;
	Game*			gm;
	Socket*			skt;

	sm = Sockets_manager::GetInstance();
	gm = Game::GetInstance();
	env = Env::GetInstance();
	if (sm->GetHostSocket() == NULL)
	{
		if (gm->InitLobby(env->hostScreen->inputMap.GetText(), 0) == 0)
		{
			skt = sm->ListenSocket(PORT);
			if (skt != NULL)
			{
				gm->players[0]->SetSocket(skt);
				sm->SetHostSocket(skt);
				std::cout << "Listen[" << PORT << "] by ["
					<< skt->fd
					<< "]" << std::endl;
				env->SwitchScreen(env->lobbyScreen);
			}
			else
			{
				gm->CleanAll();
				std::cerr << "Host Socket failed." << std::endl;
			}
		}
		else
			std::cerr << "Init Lobby failed." << std::endl;
	}
}

void	__Host_Screen__backCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "back\n";
	Env*			env;

	env = Env::GetInstance();
	env->SwitchScreen(env->mainScreen);
}

Host_Screen::Host_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT),
			     hostTitle(DEFAULT_FONT_PATH, 40,
				       (char*)"Host Server",
				       (ID::Color*)&(ID::COLOR_GOLD)),
			     inputMap(500, 30, UI_FLAGS, UI_RMASK,
				      UI_GMASK, UI_BMASK, UI_AMASK,
				      DEFAULT_FONT_PATH, 20,
				      (ID::Color*)&(ID::COLOR_ORANGE)),
			     hostGame(160, 30, UI_FLAGS, UI_RMASK,
				      UI_GMASK, UI_BMASK, UI_AMASK,
				      DEFAULT_FONT_PATH, 20,
				      (char*)"Host Game",
				      (ID::Color*)&(ID::COLOR_GOLD)),
			     back(80, 30, UI_FLAGS, UI_RMASK,
				  UI_GMASK, UI_BMASK, UI_AMASK,
				  DEFAULT_FONT_PATH, 20, (char*)"Back",
				  (ID::Color*)&(ID::COLOR_GOLD))
{
	this->SetName("Host_Screen");

	this->hostTitle.SetPos(15, 15);
	this->AddChild(&(this->hostTitle));

	this->hostGame.SetPos(ID::REL_POS_CENTER);
	this->hostGame.SetCbClickDown(__Host_Screen__hostGameCbClickDown, NULL);
	this->AddChild(&(this->hostGame));

	this->inputMap.SetPos(50, 300);
	this->inputMap.SetBackground(0xFFFFFFFF);
//	this->inputMap.SetMaxLength(50);
	this->AddChild(&(this->inputMap));

	this->back.SetPos(ID::REL_POS_BOT);
	this->back.SetCbClickDown(__Host_Screen__backCbClickDown, NULL);
	this->AddChild(&(this->back));
}

Host_Screen::~Host_Screen()
{
}

void	Host_Screen::ApplyEvent(ID::Event* e)
{
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		Sockets_manager*	sm;

		sm = Sockets_manager::GetInstance();
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			__Host_Screen__hostGameCbClickDown(0, 0, 0);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
		{
			__Host_Screen__backCbClickDown(0, 0, 0);
		}
	}
}

void	Host_Screen::Refresh()
{
	Sockets_manager*	sm;

	sm = Sockets_manager::GetInstance();
	if (sm->AcceptFromHost() == 1)
		std::cout << "Add Client\n";
}

void	Host_Screen::Enter()
{
	Env*	e;

	std::cout << "Entering HostScreen\n";
	e = Env::GetInstance();
	e->window->SetDrawableFocused(&(e->hostScreen->inputMap));
}

void	Host_Screen::Leave()
{
	Env*	e;
	std::cout << "Leaving HostScreen\n";

	e = Env::GetInstance();
	e->window->SetDrawableFocused(NULL);
}

