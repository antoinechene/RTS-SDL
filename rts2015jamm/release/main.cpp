#include "ID_glibrary.h"
#include "Threads/Thread_Gameplay.h"
#include "Threads/Thread_Display.h"
#include "Screens/Abstract_Screen.hpp"
#include "Network/Protocol_Manager.h"
#include "Network/Sockets_manager.h"
#include "Network/Socket.h"
#include "Database/Database.h"
#include "Game/Scripts/Script_Manager.h"
#include "Sound/Sounds_mix.h"
#include "Env.h"
#include "Constantes.h"
#include <iostream>

#include "Game/Game.h"
#include "Game/Cam.h"
#include "Screens/Game_Screen.h"
#include "Screens/Main_Screen.h"
#include "Game/Data_Access.h"
#include "Game/Sprite_Anime.h"
#include "Screens/Option_Screen.h"

void	__main_winKeyUp(ID::Control*, ID::Event* e, void*)
{
	Env*	env;

	env = Env::GetInstance();
	if (env->focusScreen != NULL)
		env->focusScreen->ApplyEvent(e);
}

void	__main_winKeyDown(ID::Control*, ID::Event* e, void*)
{
	Env*	env;

	env = Env::GetInstance();
	if (e->keyDown.keysym.sym == ID::KEY_F10)
	{
		Game*	g;
		Cam*	c;

		g = Game::GetInstance();
		c = Cam::GetInstance();
		env->SwitchScreen(env->mainScreen);
		g->InitLobby((char*)"map1", 0);
		g->InitGame();
		Sounds::GetInstance()->play_sound(SOUND_THEME_MENU);
		g->StartGame();
	}
	if (e->keyDown.keysym.sym == ID::KEY_F11)
	{
		env->gameScreen->DisplayResult(0);
	}
	else if (e->keyDown.keysym.sym == ID::KEY_p)
	{
		if (!Sounds::GetInstance()->pause_audio_player())
			Sounds::GetInstance()->unpause_audio_player();
	}
	else
		check_shortcut(e, env->optionScreen);
	if (env->focusScreen != NULL)
		env->focusScreen->ApplyEvent(e);
}

inline void	CheckNetwork(Sockets_manager* sm, Protocol_Manager* pm)
{
	fd_set				fdread;
	fd_set				fdwrite;
	int				fdmax;
	struct timeval			timeout;
	std::vector<Socket*>*		sockets;
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;

	fdmax = 1;
	FD_ZERO(&fdwrite);
	FD_ZERO(&fdread);
	sockets = sm->GetSockets();
	pm->Write(sockets);
	it = sockets->begin();
	end = sockets->end();
	while (it != end)
	{
		FD_SET((*it)->fd, &fdread);
		if ((*it)->send.empty() == false)
			FD_SET((*it)->fd, &fdwrite);
		if ((*it)->fd >= fdmax)
			fdmax = (*it)->fd + 1;
		++it;
	}
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select(fdmax, &fdread, &fdwrite, 0, &timeout) > 0)
	{
		it = sockets->begin();
		while (it != end)
		{
			pm->workingSocket = *it;
			if (FD_ISSET((*it)->fd, &fdread))
			{
//				std::cout << "Read(" << (*it)->fd << ")\n";
				if ((*it)->Read() == -1)
				{
					sm->DeleteSocket(it);
					end = sm->GetSockets()->end();
					continue ;
				}
				pm->Parse(&((*it)->read));
			}
			if (FD_ISSET((*it)->fd, &fdwrite))
			{
//				std::cout << "Write(" << (*it)->fd << ")\n";
				(*it)->Write();
			}
			++it;
		}
	}
}

inline int	propagateEvent(ID::Window* w, ID::Event* e)
{
	if (e->type == ID::EVENT_MOTION)
		w->Motion(e);
	else if (e->type == ID::EVENT_CLICK_DOWN)
		w->ClickDown(e);
	else if (e->type == ID::EVENT_CLICK_UP)
		w->ClickUp(e);
	else if (e->type == ID::EVENT_KEY_UP)
		w->KeyUp(e);
	else if (e->type == ID::EVENT_KEY_DOWN)
		w->KeyDown(e);
	else if (e->type == ID::EVENT_QUIT)
		return 1;
	return 0;
}

int	InitInstances(ID::Window* w)
{
	if (ID::Wrapper_Event::GetInstance() == NULL)
		return -1;

	if (Database::GetInstance() == NULL)
		return -1;
	if (Database::GetInstance()->Init_ResourceDB() == -1)
		throw 17;
	if (Env::GetInstance() == NULL)
		return -1;
	Env::GetInstance()->Init(w);

	if (Sounds::GetInstance() == NULL)
		return -1;
	Sounds::GetInstance()->init();
//	Sounds::GetInstance()->set_theme(SONG_DRS);

	if (Sockets_manager::GetInstance() == NULL)
		return -1;
	if (Protocol_Manager::GetInstance() == NULL)
		return -1;

	if (Script_Manager::GetInstance() == NULL)
		return -1;

	if (Data_Access::GetInstance() == NULL)
		return -1;
	if (Data_Access::GetInstance()->Init() == -1)
		throw 12;

	if (Database::GetInstance()->Init_ProductionDB() == -1)
		throw 18;
	if (Script_Manager::GetInstance()->Init() == -1)
		return -1;
	if (Database::GetInstance()->Init_UnitDB() == -1)
		throw 13;

	if (Game::GetInstance() == NULL)
		return -1;
	if (Map::GetInstance() == NULL)
		return -1;

	if (Cam::GetInstance() == NULL)
		return -1;
	if (Cam::GetInstance()->Init(w) == -1)
		throw 14;

	if (Thread_Gameplay::GetInstance() == NULL)
		return -1;
	if (Thread_Gameplay::GetInstance()->Init() != 0)
		throw 15;

	if (Thread_Display::GetInstance() == NULL)
		return -1;
	if (Thread_Display::GetInstance()->Init() == -1)
		throw 16;
	return 0;
}

void	CleanProgram(ID::Window* w)
{
	Thread_Display::GetInstance()->Destroy();
	Thread_Display::GetInstance()->Kill();
	Thread_Gameplay::GetInstance()->Destroy();
	Thread_Gameplay::GetInstance()->Kill();
	Cam::GetInstance()->Kill();
	Map::GetInstance()->Kill();
	Game::GetInstance()->Kill();
	Database::GetInstance()->Kill();
	Data_Access::GetInstance()->Kill();
	Sockets_manager::GetInstance()->Kill();
	Protocol_Manager::GetInstance()->Kill();
	Sounds::GetInstance()->stop_audio_player();
	Sounds::GetInstance()->Kill();
	Env::GetInstance()->Kill();

	delete w;
	ID::DestroyWrappers();
	ID::Quit();
	ID::About();
}

int	main()
{
	if (ID::Init(SDL_INIT_VIDEO) == -1)
	{
		std::cout << "Fatal Error: ID::Init()" << std::endl;
		return -1;
	}
	ID::Window*		win;
	ID::Wrapper_Event*	we;
	ID::Event		event;
	Env*			env;
	Sockets_manager*	sktM;
	Protocol_Manager*	pm;

	try
	{
		win = new ID::Window(UI_WIDTH, UI_HEIGHT,
				     SDL_SWSURFACE /*| SDL_NOFRAME*/);
		if (win == NULL)
		{
			std::cerr << "Fatal Error: ID::Window()" << std::endl;
			return -1;
		}
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
				SDL_DEFAULT_REPEAT_INTERVAL);
		win->SetCbKeyUp(__main_winKeyUp, NULL);
		win->SetCbKeyDown(__main_winKeyDown, NULL);
		if (ID::InitWrappers() == -1)
		{
			std::cerr << "Fatal Error: ID::InitWrappers()" << std::endl;
			delete win;
			ID::Quit();
			return -1;
		}
		if (InitInstances(win) == -1)
			throw 1;
	}
	catch (int e)
	{
		std::cerr << "Error " << e << ": Cannot init program" << std::endl;
		CleanProgram(win);
		return -1;
	}

	env = Env::GetInstance();
	we = ID::Wrapper_Event::GetInstance();
	sktM = Sockets_manager::GetInstance();
	pm = Protocol_Manager::GetInstance();
	while (env->alive == true)
	{
		CheckNetwork(sktM, pm);
		if (we->PollEvent(&event))
		{
			pthread_mutex_lock(&(env->displayMutex));
			if (propagateEvent(win, &event) == 1)
				env->alive = false;
			pthread_mutex_unlock(&(env->displayMutex));
		}
		usleep(10);
	}
	CleanProgram(win);
	return 0;
}
