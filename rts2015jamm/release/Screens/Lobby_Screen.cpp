#include "Lobby_Screen.h"
#include "../Env.h"
#include "../Constantes.h"
#include "../Network/Sockets_manager.h"
#include "../Network/Protocol_Manager.h"
#include "../Network/Socket.h"
#include "Main_Screen.h"
#include <iostream>
#include "../Game/Game.h"
#include "../Game/Player.h"
#include <stdlib.h>

void	__Lobby_Screen_Player__updateCbClickDown(ID::Control*, ID::Event*, void* data)
{
	Lobby_Player_Line*	info = (Lobby_Player_Line*)data;
	Game*			gm;
	Protocol_Manager*	pm;
	int			i;

	pm = Protocol_Manager::GetInstance();
	gm = Game::GetInstance();
	i = 0;

	i = atoi(info->teamInput.GetText());
	if (i >= 0 && i < gm->countPlayers)
		pm->prot.AddCmdLobbyUpdID(gm->myID, i);
/*
		info->player->SetTeam(i);
	else
		info->player->SetTeam(0);
*/
	i = atoi(info->factionInput.GetText());
	if (i >= 0 && i < gm->countPlayers)
		pm->prot.AddCmdLobbyUpdFaction(gm->myID, i);
/*		info->player->SetFaction(i);
	else
		info->player->SetFaction(0);
*/
	std::cout << info->player->GetName() << std::endl;
	std::cout << " Ask Team    : " << (int)info->player->GetTeam() << std::endl;
	std::cout << " Ask Faction : " << (int)info->player->GetFaction() << std::endl;
}


Lobby_Player_Line::Lobby_Player_Line(Player* assignedPlayer) : Drawable(230, 40,
							UI_FLAGS, UI_RMASK,
							UI_GMASK, UI_BMASK, UI_AMASK),
						playerName(DEFAULT_FONT_PATH, 20,
							   assignedPlayer->GetName(), (ID::Color*)&(ID::COLOR_GOLD)),
					      teamInput(20, 20, UI_FLAGS, UI_RMASK,
							UI_GMASK, UI_BMASK, UI_AMASK,
							DEFAULT_FONT_PATH, 20,
							(ID::Color*)&ID::COLOR_ORANGE),
					      colorInput(20, 20, UI_FLAGS, UI_RMASK,
							UI_GMASK, UI_BMASK, UI_AMASK,
							DEFAULT_FONT_PATH, 20,
							(ID::Color*)&ID::COLOR_ORANGE),
					      factionInput(20, 20, UI_FLAGS, UI_RMASK,
							UI_GMASK, UI_BMASK, UI_AMASK,
							DEFAULT_FONT_PATH, 20,
							(ID::Color*)&ID::COLOR_ORANGE),
					      update(130, 20, UI_FLAGS, UI_RMASK,
							UI_GMASK, UI_BMASK, UI_AMASK,
							DEFAULT_FONT_PATH, 10, (char*)"[update]",
							(ID::Color*)&(ID::COLOR_GOLD))
{
	Env*	env;
	Game*	game;

	env = Env::GetInstance();
	game = Game::GetInstance();

//	this->playerName.SetPos(x, y);
	this->AddChild(&(this->playerName));

	this->teamInput.SetPos(25, 20);
	this->teamInput.SetMaxLength(1);
	if (assignedPlayer->GetID() == game->myID)
	{
		this->teamInput.SetBackground(0xFFFFFFFF);
		this->teamInput.SetEnable(true);
	}
	else
		this->teamInput.SetEnable(false);
	this->AddChild(&(this->teamInput));

	this->colorInput.SetPos(50, 20);
	if (assignedPlayer->GetID() == game->myID)
	{
		this->colorInput.SetBackground(0xFFFFFFFF);
		this->colorInput.SetMaxLength(1);
		this->AddChild(&(this->colorInput));
	}
	else
		this->teamInput.SetEnable(false);
	this->factionInput.SetPos(75, 20);

	if (assignedPlayer->GetID() == game->myID)
	{
		this->factionInput.SetBackground(0xFFFFFFFF);
		this->factionInput.SetMaxLength(1);
		this->AddChild(&(this->factionInput));
	}
	else
		this->teamInput.SetEnable(false);

	if (assignedPlayer->GetID() == game->myID)
	{
		this->update.SetPos(100, 20);
		this->update.SetCbClickDown(__Lobby_Screen_Player__updateCbClickDown, this);
		this->AddChild(&(this->update));
	}

	this->player = assignedPlayer;
}

Lobby_Player_Line::~Lobby_Player_Line()
{
	Env*	env;

	env = Env::GetInstance();
	env->lobbyScreen->DelChild(this);
/*	env->lobbyScreen->DelChild(&(this->playerName));
	env->lobbyScreen->DelChild(&this->teamInput);
	env->lobbyScreen->DelChild(&this->colorInput);
	env->lobbyScreen->DelChild(&this->factionInput);
	env->lobbyScreen->DelChild(&(this->update));*/
}

void	__Lobby_Screen__startGameCbClickDown(ID::Control*, ID::Event*, void*)
{
	Protocol_Manager*	pm;
	std::cout << "StartGame\n";

	pm = Protocol_Manager::GetInstance();
	pm->prot.AddCmdStartListen();
}

void	__Lobby_Screen__backCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "back\n";
	Env*	env;
	Game*	gm;

	gm = Game::GetInstance();
	gm->CleanAll();
	env = Env::GetInstance();
	env->SwitchScreen(env->mainScreen);
}

Lobby_Screen::Lobby_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT),
			     lobbyTitle(DEFAULT_FONT_PATH, 40,
				       (char*)"Game Lobby",
				       (ID::Color*)&(ID::COLOR_GOLD)),
			     startGame(160, 30, UI_FLAGS, UI_RMASK,
				       UI_GMASK, UI_BMASK, UI_AMASK,
				       DEFAULT_FONT_PATH, 20,
				       (char*)"Start Game",
				       (ID::Color*)&(ID::COLOR_GOLD)),
			     back(80, 30, UI_FLAGS, UI_RMASK,
				  UI_GMASK, UI_BMASK, UI_AMASK,
				  DEFAULT_FONT_PATH, 20, (char*)"Back",
				  (ID::Color*)&(ID::COLOR_GOLD))
{
	Env*	env;

	env = Env::GetInstance();

	this->SetName("Lobby_Screen");

	this->lobbyTitle.SetPos(15, 15);
	this->AddChild(&(this->lobbyTitle));

	this->startGame.SetPos(ID::REL_POS_CENTER);
	this->startGame.SetCbClickDown(__Lobby_Screen__startGameCbClickDown, NULL);
	this->AddChild(&(this->startGame));

	this->back.SetPos(ID::REL_POS_BOT);
	this->back.SetCbClickDown(__Lobby_Screen__backCbClickDown, NULL);
	this->AddChild(&(this->back));
}

int	Lobby_Screen::AddPlayer(Player* player)
{
	Game*			gameEnv;
	Lobby_Player_Line*	addme;

	std::cout << "My ID : " << (int)player->GetID() << std::endl;
	gameEnv = Game::GetInstance();
	addme = new Lobby_Player_Line(player);
	if (addme == NULL)
		return -1;
	addme->SetPos((350 * ((int)player->GetID() / 5)) + 30,
		      (100 * ((int)player->GetID() % 5)) + 100);
	this->AddChild(addme);
	this->players.push_back(addme);
	return 0;
}

void	Lobby_Screen::DelPlayer(Player* p)
{
	std::vector<Lobby_Player_Line*>::iterator	it;
	std::vector<Lobby_Player_Line*>::iterator	end;

	it = this->players.begin();
	end = this->players.end();
	while (it != end)
	{
		if ((*it)->player == p)
		{
			delete *it;
			this->players.erase(it);
			return ;
		}
		++it;
	}
}

void	Lobby_Screen::UpdatePlayerID(Player* p)
{
	std::vector<Lobby_Player_Line*>::iterator	it;
	std::vector<Lobby_Player_Line*>::iterator	end;

	it = this->players.begin();
	end = this->players.end();
	std::cout << "UpdPlayerID" << (int)p->GetID() << std::endl;
	while (it != end)
	{
		if ((*it)->player == p)
		{
			(*it)->SetPos((350 * ((int)(*it)->player->GetID() / 5)) + 30,
				      (100 * ((int)(*it)->player->GetID() % 5)) + 100);
			return ;
		}
		++it;
	}
}

Lobby_Screen::~Lobby_Screen()
{
}/*void	Lobby_Screen::DisplayPlayers()
{
	Game*		gameEnv;

	gameEnv = Game::GetInstance();

	if (gameEnv->players != NULL)
	{
		this->player1.GetLabel()->SetText(gameEnv->players->GetName());
	}
}*/

void	Lobby_Screen::ApplyEvent(ID::Event* e)
{
	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
		{
			__Lobby_Screen__startGameCbClickDown(0, 0, 0);
		}
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
		{
			__Lobby_Screen__backCbClickDown(0, 0, 0);
		}
	}
}

void	Lobby_Screen::Refresh()
{
	Game*			gm;
	Sockets_manager*	sm;

	gm = Game::GetInstance();
	sm = Sockets_manager::GetInstance();
	if (gm->AmIHost == false)
	{
		if (sm->GetHostSocket() == NULL)
		{
			gm->CleanAll();
			std::cout << "Host has been disconnected.\n";
			Env::GetInstance()->SwitchScreen(Env::GetInstance()->mainScreen);
			return ;
		}
	}
	if (sm->AcceptFromHost() == 1)
		std::cout << "Add Client\n";
}

void	Lobby_Screen::Enter()
{
	std::cout << "Entering LobbyScreen\n";
	Game*	gm;

	gm = Game::GetInstance();
	gm->SetInLobby(true);
	if (gm->AmIHost == true)
		this->startGame.SetOnScreen(true);
	else
		this->startGame.SetOnScreen(false);
}

void	Lobby_Screen::Leave()
{
	std::cout << "Leaving LobbyScreen\n";
	Game*	gm;

	gm = Game::GetInstance();
	gm->SetInLobby(false);
}

void	Lobby_Screen::ClearPlayers(void)
{
	std::vector<Lobby_Player_Line*>::iterator	it;
	std::vector<Lobby_Player_Line*>::iterator	end;

	it = this->players.begin();
	end = this->players.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
}
