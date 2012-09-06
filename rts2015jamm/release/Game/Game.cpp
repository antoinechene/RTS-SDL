#include "Game.h"
#include "Map.h"
#include "Unit.h"
#include "Cam.h"
#include "Player.h"
#include "Scripts/Script.hpp"
#include "../Env.h"
#include "../Database/Database.h"
#include "../Network/Sockets_manager.h"
#include "../Network/Protocol_Manager.h"
#include "../Screens/Lobby_Screen.h"
#include "../Screens/Game_Screen.h"
#include "../ID_glibrary_Extension/Light_Drawable.h"
#include <stdlib.h>
#include <sys/time.h>

Game::Game() : currentTick(0),
	       playerFocusScript(NULL),
	       playerFocusProd(NULL),
	       shadowProd(NULL),
	       myID(0),
	       AmIHost(false),
	       countPlayers(0),
	       players(NULL),
	       isInLobby(false),
	       isGameStarted(false),
	       lastSelection(0)
{
	struct timeval	tv;

	if (pthread_mutex_init(&(this->__gameMutex), NULL) != 0)
		throw 368;
	if (gettimeofday(&tv, NULL) == -1)
		throw 369;
	srand(tv.tv_usec);
}

Game::~Game()
{
	pthread_mutex_destroy(&(this->__gameMutex));
	delete[] this->players;
}

int	Game::AddPlayer(char* name, int ID)
{
	int	i;
	Env*	env;

	env = Env::GetInstance();

	this->LockMutex();
	if (ID != -1)
	{
		if (this->players[ID] == NULL)
		{
			// ajouter Try catch a player
			this->players[ID] = new Player(ID, name, 0, 0);
			if (this->players[ID] == NULL)
			{
				this->UnlockMutex();
				return -1;
			}
	/*		if (env->lobbyScreen->AddPlayer(this->players[ID]) == -1)
			{
				std::cerr << "Player[" << ID << "] error." << std::endl;
				delete this->players[ID];
				this->players[ID] = NULL;
			}
			else
	*/			std::cout << "Player[" << ID << "] added.\n";
			this->UnlockMutex();
			return ID;
		}
		this->UnlockMutex();
		return -1;
	}
	i = 0;
	while (i < this->countPlayers)
	{
		if (this->players[i] == NULL)
		{
			this->players[i] = new Player(i, name, 0, 0);
			if (this->players[i] == NULL)
			{
				this->UnlockMutex();
				return -1;
			}
		//	env->lobbyScreen->AddPlayer(this->players[i]);
			std::cout << "Player[" << i << "] added.\n";
			this->UnlockMutex();
			return i;
		}
		++i;
	}
	this->UnlockMutex();
	return -1;
}

void	Game::DelPlayer(int ID)
{
	Env*	e;

	e = Env::GetInstance();
	delete this->players[ID];
	this->players[ID] = NULL;
}

int	Game::UpdatePlayerFaction(uint8_t ID, uint8_t faction)
{
	this->LockMutex();
	if (this->players[ID] != NULL) // Add check Force Faction
	{
//		if (this->AmIHost == true)
//			Protocol_Manager::GetInstance()->prot.AddCmdLobbyUpdFaction(ID, faction);
		this->players[ID]->SetFaction(faction);
		this->UnlockMutex();
		return 1;
	}
	this->UnlockMutex();
	return 0;
}

int	Game::UpdatePlayerID(uint8_t ID, uint8_t newID)
{
	this->LockMutex();
	if (this->players[ID] != NULL &&
	    this->players[newID] == NULL)
	{
//		if (this->AmIHost == true)
//			Protocol_Manager::GetInstance()->prot.AddCmdLobbyUpdID(ID, newID);
		if (ID == this->myID)
			this->myID = newID;
		this->players[ID]->SetID(newID);
		this->players[newID] = this->players[ID];
		this->players[ID] = NULL;
		Env::GetInstance()->lobbyScreen->UpdatePlayerID(this->players[newID]);
		this->UnlockMutex();
		return 1;
	}
	this->UnlockMutex();
	return 0;
}

int	Game::__CheckInitialPos(Map* m)
{
	std::vector<unsigned char>::iterator	itIP;
	std::vector<unsigned char>::iterator	endIP;
	std::vector<unsigned char>	pfIP;
	Map::InitialPosition*	IPs;
	int			i;
	int			j;

	IPs = NULL;
	while (IPs == NULL)
		IPs = new Map::InitialPosition[m->countInitialPos];
	i = 0;
	while (i < m->countInitialPos)
	{
		IPs[i].x = 0;
		IPs[i].y = 0;
		++i;
	}

	i = 0;
	while (i < m->countInitialPos) // Check clone IP
	{
		j = 0;
		while (IPs[j].x != 0 || IPs[j].y != 0)
		{
			if (IPs[j].x == m->initialPos[i].x &&
			    IPs[j].y == m->initialPos[i].y)
			{
				delete IPs;
				return -1;
			}
			++j;
		}
		IPs[i].x = m->initialPos[i].x;
		IPs[i].y = m->initialPos[i].y;
		++i;
	}

	i = 0;
	while (i < m->countTeams) // Check Player same fIP
	{
		j = 0;
		while (j < m->teams[i].countPlayers)
		{
			if (m->teams[i].players[j].forceInitialPos > m->countInitialPos)
				return -1;
			else if (m->teams[i].players[j].forceInitialPos != 0)
			{
				itIP = pfIP.begin();
				endIP = pfIP.end();
				while (itIP != endIP)
				{
					if (m->teams[i].players[j].forceInitialPos == *itIP)
						return -1;
					++itIP;
				}
				pfIP.push_back(m->teams[i].players[j].forceInitialPos);
			}
			++j;
		}
		++i;
	}
	delete IPs;
	return 0;
}

int	Game::InitLobby(char* filename, int ID)
{
	Cam*	cam;
	Map*	map;
	int	i;
	int	n;

	this->LockMutex();
	map = Map::GetInstance();
	if (map->Load(filename) == -1)
	{
		this->UnlockMutex();
		return -1;
	}
	i = 0;
	n = 0;
	while (i < map->countTeams)
	{
		n = n + map->teams[i].countPlayers;
		++i;
	}
	// Check Map settings
	if (n <= 0 ||
	    map->countInitialPos < n ||
	    map->w <= 5 ||
	    map->h <= 5 ||
	    this->__CheckInitialPos(map))
	{
		this->UnlockMutex();
		std::cerr << "Map corrupted." << std::endl;
		return -1;
	}
	this->countPlayers = n;
	this->players = new Player*[n];
	if (this->players == NULL)
	{
		this->UnlockMutex();
		this->CleanAll();
		return -1;
	}
	while (n > 0)
	{
		--n;
		this->players[n] = NULL;
	}
	this->AmIHost = true;
	this->myID = ID;
	this->UnlockMutex();
	if (this->AddPlayer(Env::GetInstance()->nickname, ID) == -1)
	{
		this->CleanAll();
		return -1;
	}
	this->players[ID]->SetHosting(true);
	this->players[ID]->SetConnectedToAll(true);

	cam = Cam::GetInstance();
	cam->SetMapData(map);
	if (cam->InitMapDrawable() == -1)
	{
		this->CleanAll();
		return -1;
	}
	Env::GetInstance()->gameScreen->RefreshMinimap();
	return 0;
}

int	Game::InitGame(void)
{
	std::list<uint8_t>::iterator	itIP;
	std::list<uint8_t>::iterator	endIP;
	std::list<uint8_t>		IPs;
	Protocol_Manager*		pm;
	Map*				m;
	Env*				e;
	int				i;
	int				j;

	e = Env::GetInstance();
	m = Map::GetInstance();
	pm = Protocol_Manager::GetInstance();
	if (this->AmIHost == false)
		return -1;

	i = 1;
	while (i <= m->countInitialPos)
	{
		std::cout << "IP(" << i << ")\n";
		IPs.push_back(i);
		i++;
	}

	i = 0;
	while (i < m->countTeams) // Retrieve player's fIP
	{
		j = 0;
		while (j < m->teams[i].countPlayers)
		{
			if (m->teams[i].players[j].forceInitialPos != 0)
			{
				itIP = IPs.begin();
				endIP = IPs.end();
				while (itIP != endIP)
				{
					if (*itIP == m->teams[i].players[j].forceInitialPos)
					{
						std::cout << "RETIP(" << (int)*itIP << ")\n";
						IPs.erase(itIP);
						break ;
					}
					++itIP;
				}
			}
			++j;
		}
		++i;
	}

	i = 0;
	while (i < this->countPlayers) // Share IP Randomly
	{
		if (this->players[i] != NULL)
		{
			itIP = IPs.begin();
			endIP = IPs.end();
			j = rand() % IPs.size();
			std::cout << "Randj(" << j << ")\n";
			while (j > 0)
			{
				itIP++;
				--j;
			}
			std::cout << "GIVEIP(" << (int)*itIP << ")\n";
			this->players[i]->SetInitialPos(*itIP - 1);
			IPs.erase(itIP);
		}
		++i;
	}
	return 0;
}

int	Game::StartGame(void)
{
	std::cout << "Game::StartGame()\n";
	Map::InitialPosition*	IP;
	Protocol_Manager*	pm;
	Database*		db;
	Env*			e;
	Cam*			c;
	Map*			m;

	pm = Protocol_Manager::GetInstance();
	db = Database::GetInstance();
	e = Env::GetInstance();
	m = Map::GetInstance();
	c = Cam::GetInstance();

	IP = &(m->initialPos[this->GetMe()->GetInitialPos()]);
	if (this->GetMe()->GetFaction() == 0)
		pm->prot.AddCmdGameCreateUnit(1, IP->x, IP->y, this->myID);
	else if (this->GetMe()->GetFaction() == 1)
		pm->prot.AddCmdGameCreateUnit(4, IP->x, IP->y, this->myID);
	else if (this->GetMe()->GetFaction() == 2)
		pm->prot.AddCmdGameCreateUnit(7, IP->x, IP->y, this->myID);
	c->SetCam(IP->x * CASE_SIDE_PIXELS, IP->y * CASE_SIDE_PIXELS);
	c->Refresh();
	std::cout << "SetCam(" << IP->x << "," << IP->y << ")\n";
	e->SwitchScreen(e->gameScreen);
	this->SetGameRunning(true);
	return 0;
}

void	Game::CleanAll(void)
{
	Sockets_manager*	sm;

	sm = Sockets_manager::GetInstance();
	sm->ClearSockets();
	this->LockMutex();
	this->SetInLobby(false);
	this->SetGameRunning(false);
	this->currentTick = 0;
	this->myID = 0;
	this->AmIHost = false;
	this->UnlockMutex();
	while (this->countPlayers != 0)
	{
		--(this->countPlayers);
		delete this->players[this->countPlayers];
	}
	delete[] this->players;
	this->players = NULL;
	std::cout << "Game::CleanAll()\n";
}

bool	Game::ArePlayersReady(void)
{
	int	i;

	i = 0;
	while (i < this->countPlayers)
	{
		if (this->players[i] != NULL)
		{
			if (this->players[i]->GetReadyTick() == false)
				return false;
		}
		++i;
	}
	return true;
}

void	Game::ResetPlayersReady(void)
{
	int	i;

	i = 0;
	while (i < this->countPlayers)
	{
		if (this->players[i] != NULL)
			this->players[i]->SetReadyTick(false);
		++i;
	}
}

int	Game::CheckEndGame(void)
{
	std::vector<Unit*>*		units;
	std::vector<Unit*>::iterator	it;
	std::vector<Unit*>::iterator	end;
	std::vector<uint8_t>		playersAlive;
	bool				dead;
	int				i;
	int				j;

	if (1 == 1) //#TODO Bool si la regle est activee, faudrait creer une classe config
	{
		// Check building rule
		i = 0;
		while (i < this->countPlayers)
		{
			if (this->players[i] == NULL)
			{
				++i;
				continue ;
			}
			dead = true;
			units = this->players[i]->GetUnits();
			it = units->begin();
			end = units->end();
			while (it != end)
			{
				if (*it != NULL && (*it)->GetModel()->GetType() >= 3)
				{
					//#TODO En attendant de gerer les teams, FFA.
					if (playersAlive.size() == 1)
						return 0;
					playersAlive.push_back(i);
					dead = false;
					break ;
				}
				++it;
			}
			if (dead == true)
			{
				if (i == this->myID)
				{
					return 1; // I'VE LOST
				}
				else
				{
					//#TODO Retirer le player de la liste? A voir
				}
			}
			++i;
		}
		//#TODO Si on gere les teams, il faut la partie ci-dessous.
		if (1 == 2)
		{
			dead = true; // Win team variable
			i = 0;
			j = playersAlive.size();
			while (i < j)
			{
				if (this->players[playersAlive[i]]->GetTeam() != this->GetMe()->GetTeam())
				{
					dead = false;
					break ;
				}
				++i;
			}
			if (dead == true)
			{
				// 1 team restante, victoire
				return 2; // 2 == I'VE WON
			}
		}
		else
		{
			if (playersAlive.size() == 1)
			{
				return 2;
			}
		}
	}
	return 0; // Nothing happened
}
