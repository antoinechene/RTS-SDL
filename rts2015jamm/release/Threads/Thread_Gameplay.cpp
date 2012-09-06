#include "Thread_Gameplay.h"
#include "../Env.h"
#include "../Network/Protocol_Manager.h"
#include "../Game/Game.h"
#include "../Game/Cam.h"
#include "../Game/Player.h"
#include "../Game/Unit.h"
#include "../Game/Scripts/Script.hpp"
#include "../Game/Game_Events/GameEvent.hpp"
#include "../Screens/Game_Screen.h"
#include "../ID_glibrary_Extension/Map_Drawable.h"
#include <sys/time.h>

Thread_Gameplay::Thread_Gameplay() : alive(true)
{
}

Thread_Gameplay::~Thread_Gameplay()
{
}

int	Thread_Gameplay::Init(void)
{
	this->alive = true;
	if (pthread_create(&(this->idThread), NULL, this->Run, this) != 0)
		return -1;
	return 0;
}

void	Thread_Gameplay::Destroy()
{
	this->alive = false;
	if (pthread_join(this->idThread, NULL) != 0)
	{
		std::cerr << "Error pthread_join Thread_Astar" << std::endl;
	}
}

void*	Thread_Gameplay::Run(void* _ta)
{
	Thread_Gameplay*	ta;
	Game*			game;
	Cam*			cam;
	Protocol_Manager*	pm;
	pthread_mutex_t*	dispMtx;
	uint32_t		interval;
	uint8_t			i;
	struct timeval		nextTick;
	struct timeval		current;

	ID::vector<GameEvent*>::iterator	itE;
	ID::vector<GameEvent*>::iterator	endE;
	ID::vector<Script*>::iterator		itS;
	ID::vector<Script*>::iterator		endS;
	std::vector<Unit*>*			units;
	std::vector<Unit*>::iterator		itU;
	std::vector<Unit*>::iterator		endU;

	game = Game::GetInstance();
	cam = Cam::GetInstance();
	pm = Protocol_Manager::GetInstance();
	ta = (Thread_Gameplay*)_ta;
	dispMtx = &(Env::GetInstance()->displayMutex);
	interval = 1000000 / MAX_TPS;
	while (ta->alive == true)
	{
		usleep(1000);
		while (game->IsGameRunning() == true && ta->alive == true)
		{
			usleep(20);
				pthread_mutex_lock(dispMtx);
				itE = game->gameEvents.begin();
				endE = game->gameEvents.end();
				while (itE != endE)
				{
					if (game->currentTick == (*itE)->tick)
					{
						(*itE)->Apply();
						*itE = game->gameEvents.back();
						game->gameEvents.pop_back();
					}
					++itE;
				}

			game->LockMutex();
			{
//			if (game->TrylockMutex())
				if (game->players != NULL &&
				    game->players[game->myID] != NULL &&
				    game->players[game->myID]->GetReadyTick() == false)
					pm->prot.AddCmdGameEndTick();

				if (gettimeofday(&current, NULL) == 0 &&
				    timercmp(&nextTick, &current, <) && // Time is up
				    game->ArePlayersReady() == true) // ALL's ready
				{
//					std::cout << "Tick="
//						<< game->currentTick
//						<< std::endl;
					cam->GetMapDrawable()->SetInvalidate(true);
					i = 0;
					while (i < game->countPlayers)
					{
						if (game->players[i] != NULL)
						{
							units = game->players[i]->GetUnits();
							itU = units->begin();
							endU = units->end();
							while (itU != endU)
							{
								if (*itU != NULL)
									(*itU)->Update();
								++itU;
							}
						}
						++i;
					}

					itS = game->runningScripts.begin();
					endS = game->runningScripts.end();
					while (itS != endS)
					{
						(*itS)->Apply();
						++itS;
					}

					game->ResetPlayersReady();
					++game->currentTick;

					nextTick.tv_sec = current.tv_sec;
					nextTick.tv_usec = current.tv_usec + interval;
					if (current.tv_usec >= 1000000)
					{
						nextTick.tv_sec += 1;
						nextTick.tv_usec -= 1000000;
					}
				}
				pthread_mutex_unlock(dispMtx);
			}
			/*i = game->CheckEndGame();
			if (i != 0)
			{
				if (i == 1)
					Env::GetInstance()->gameScreen->DisplayResult(0);
				else if (i == 2)
					Env::GetInstance()->gameScreen->DisplayResult(1);
				game->SetGameRunning(false);
				std::cout << "END I=" << (int)i << std::endl;
			}*/
			game->UnlockMutex();
		}
	}
	return 0;
}

