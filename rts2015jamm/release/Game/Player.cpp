#include "Player.h"
#include "Unit.h"
#include "../Env.h"
#include "../Game/Resource_Model.h"
#include "../Network/Socket.h"
#include "../Screens/Game_Screen.h"
#include "../Screens/Lobby_Screen.h"
#include "../Database/Database.h"
#include "../Tools.h"
#include <iostream>
/*
char*	id_itoa(int n)
{
	char*	str;
	int	e;
	int	i;

	e = 1;
	while (n / e >= 10 || n / e <= -10)
		e = e * 10;
	str = new char[12];
	if (str == 0)
		return 0;
	i = 0;
	if (n < 0)
		str[0] = '-';
	while (e != 0)
	{
		if (n >= 0)
			str[i] = ((n / e) % 10) + '0';
		else if (i != 0)
			str[i] = (n / e % 10) * -1 + '0';
		i = i + 1;
		if ((i > 1 && n < 0) || n >= 0)
			e = e / 10;
	}
	str[i] = '\0';
	return str;
}
*/
Player::Player(uint8_t ID,
	       char* name,
	       uint8_t team,
	       uint8_t faction) : _ID(ID),
				  _team(team),
				  _tokenGiver(0),
				  _faction(faction),
				  _population(0),
				  _socket(NULL),
				  _initialPos(0),
				  _readyTick(false),
				  _hosting(false),
				  _connectedToAll(false)
{
	Database*	db;
	int		i;
	Resource_Model*	rm;
	Resource	r;

	db = Database::GetInstance();
	this->_units.resize(32);
	i = 0;
	while (i < 32)
	{
		this->_units[i] = NULL;
		this->_tokensLeft.push(i);
		++i;
	}
	i = 0;
	while (i < MAX_GROUPS)
	{
		this->_groups[i].id = i;
		++i;
	}
	i = 0;
	while (i < 20 && name[i] != 0)
	{
		this->_name[i] = name[i];
		++i;
	}
	this->_name[i] = 0;
	this->_name[20] = 0;

	i = 0;
	while (i < 16)
	{
		this->_ip[i] = 0;
		++i;
	}
	if (Env::GetInstance()->lobbyScreen->AddPlayer(this) == -1)
		throw 254;
	i = 2;
	rm = db->getResourceModel(1);
	while (rm != NULL)
	{
		r.model = rm;
		r.qty = 0;
		this->_resources.push_back(r);
		rm = db->getResourceModel(i);
		++i;
	}
}

Player::~Player()
{
	std::vector<Unit*>::iterator	it;
	std::vector<Unit*>::iterator	end;

	it = this->_units.begin();
	end = this->_units.end();
	while (it != end)
	{
		if (*it != NULL)
			(*it)->Die();
		++it;
	}
	Env::GetInstance()->lobbyScreen->DelPlayer(this);
	std::cout << "Player[" << (int)this->GetID() << "] deleted." << std::endl;
}

void	Player::SetID(uint8_t data)
{
	this->_ID = data;
}

void	Player::SetName(char* data)
{
	strncpy(this->_name, data, 20);
}

void	Player::SetTeam(uint8_t data)
{
	this->_team = data;
}

void	Player::SetToken(uint32_t data)
{
	this->_tokenGiver = data;
}

void	Player::SetFaction(uint8_t data)
{
	this->_faction = data;
}

void	Player::SetSocket(Socket* data)
{
	this->_socket = data;
	this->_socket->player = this;
}

void	Player::SetIP(char* data)
{
	strncpy(this->_ip, data, 16);
}

void	Player::SetInitialPos(uint8_t IP)
{
	this->_initialPos = IP;
}

void	Player::SetHosting(bool data)
{
	this->_hosting = data;
}

void	Player::SetConnectedToAll(bool data)
{
	this->_connectedToAll = data;
}

void	Player::SetReadyTick(bool data)
{
	this->_readyTick = data;
}

void	Player::SetUnitTargetMove(uint32_t ID, double x, double y)
{
	this->_units[ID]->SetTargetMove(x, y);
}

void	Player::SetUnitTargetFollow(uint32_t ID, Unit* target)
{
	this->_units[ID]->SetTargetFollow(target);
}

void	Player::SetUnitHold(uint32_t ID)
{
	this->_units[ID]->SetHold();
}

void	Player::SetUnitTargetAttack(uint32_t ID, Unit* target)
{
	this->_units[ID]->SetTargetAttackUnit(target);
}

uint8_t	Player::GetID(void)
{
	return (this->_ID);
}

uint8_t	Player::GetTeam(void)
{
	return (this->_team);
}

uint32_t	Player::GetToken(void)
{
	return (this->_tokenGiver);
}

uint8_t	Player::GetFaction(void)
{
	return (this->_faction);
}

char*	Player::GetName(void)
{
	return this->_name;
}

Socket*	Player::GetSocket(void)
{
	return this->_socket;
}

char*	Player::GetIP(void)
{
	return this->_ip;
}

uint8_t	Player::GetInitialPos(void)
{
	return this->_initialPos;
}

bool	Player::GetHosting(void)
{
	return this->_hosting;
}

bool	Player::GetConnectedToAll(void)
{
	return this->_connectedToAll;
}

bool	Player::GetReadyTick(void)
{
	return this->_readyTick;
}

uint32_t	Player::GetPopulation(void)
{
	return this->_population;
}

void	Player::SetGroup(int idGroup)
{
	std::cout << "Player::SetGroup(" << idGroup << ")\n";
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;

	this->_groups[idGroup].ClearUnits();
	it = this->_selection.begin();
	end = this->_selection.end();
	while (it != end)
	{
		std::cout << "Group::AddUnit()\n";
		this->_groups[idGroup].AddUnit(*it);
		++it;
	}
}

void	Player::UseGroup(int idGroup)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;
	std::list<Unit*>*		units;
	uint32_t			tick;
	std::cout << "Player::UseGroup(" << idGroup << ")\n";

	units = this->_groups[idGroup].GetUnits();
	if (units->size() > 0)
	{
		this->ClearSelection();
		tick = Game::GetInstance()->currentTick;
		Game::GetInstance()->lastSelection = tick;
		it = units->begin();
		end = units->end();
		while (it != end)
		{
			std::cout << "AddUnit2Selection()\n";
			this->AddUnit2Selection(*it, tick);
			++it;
		}
	}
	Env::GetInstance()->gameScreen->RefreshSelectedList();
}

Group*	Player::GetGroup(int idGroup)
{
	return &(this->_groups[idGroup]);
}

void	Player::AddUnit2Selection(Unit* u, uint32_t last)
{
	u->SetTickSelection(last);
	this->_selection.push_back(u);
}

void		Player::DelUnitSelection(Unit* u)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;

	it = this->_selection.begin();
	end = this->_selection.end();
	while (it != end)
	{
		if (*it == u)
		{
			this->_selection.erase(it);
			return ;
		}
		++it;
	}
}

std::list<Unit*>*	Player::GetSelection(void)
{
	return &(this->_selection);
}

void		Player::ClearSelection(void)
{
	this->_selection.clear();
}

uint32_t	Player::AddUnit(Unit* unit)
{
	Game::GetInstance()->LockMutex();
	uint32_t	token;
	int		i;
	int		newLimit;

	token = this->_tokensLeft.front();
	unit->SetToken(token);
	this->_AddPopulation(unit->GetModel()->GetPopulation());
	this->_tokensLeft.pop();
	this->_units[token] = unit;
//	std::cout << "Token=" << token << std::endl;
//	std::cout << "Left=" << this->_tokensLeft.size() << std::endl;
	if (this->_tokensLeft.size() == 0)
	{
		i = this->_units.size();
		newLimit = i << 1;
		this->_units.resize(newLimit);
		while (i < newLimit)
		{
			this->_units[i] = NULL;
			this->_tokensLeft.push(i);
			++i;
		}
//		std::cout << "newLeft=" << this->_tokensLeft.size() << std::endl;
	}
	Game::GetInstance()->UnlockMutex();
	return token;
}

std::vector<Unit*>*	Player::GetUnits(void)
{
	return &(this->_units);
}

void	Player::DelUnit(uint32_t token)
{
	this->_AddPopulation(-(this->_units[token]->GetModel()->GetPopulation()));
	this->_units[token] = NULL;
	this->_tokensLeft.push(token);
}

Resource	Player::GetResource(Resource_Model* rm)
{
	std::vector<Resource>::iterator	it;
	std::vector<Resource>::iterator	end;

	it = this->_resources.begin();
	end = this->_resources.end();
	while (it != end)
	{
		if ((*it).model == rm)
			return *it;
		++it;
	}
	Resource	useless;

	useless.model = NULL;
	return useless;
}

void		Player::SetResource(Resource_Model* rm, uint32_t qty)
{
	std::vector<Resource>::iterator	it;
	std::vector<Resource>::iterator	end;

	it = this->_resources.begin();
	end = this->_resources.end();
	while (it != end)
	{
		if ((*it).model == rm)
		{
			(*it).qty = qty;
		}
		++it;
	}
}

void		Player::AddResource(Resource_Model* rm, uint32_t delta)
{
	std::vector<Resource>::iterator	it;
	std::vector<Resource>::iterator	end;
	char*				tmp;

	it = this->_resources.begin();
	end = this->_resources.end();
	while (it != end)
	{
		if ((*it).model == rm)
		{
			(*it).qty += delta;
			tmp = id_itoa((*it).qty);
			Env::GetInstance()->
				gameScreen->
				resources[(*it).model->GetID() - 1]->
				SetText(tmp);
			delete tmp;
			return ;
		}
		++it;
	}
}

void	Player::_AddPopulation(uint32_t delta)
{
	char*	pop;
	char*	max;
	char*	tmp;
	int	l;

	this->_population += delta;

	max = id_itoa(MAX_POPULATION);
	l = strlen(max) + 2;

	pop = id_itoa(this->_population);
	l += strlen(pop);

	tmp = new char[l];
	if (tmp != NULL)
	{
		strcpy(tmp, pop);
		strcat(tmp, "/");
		strcat(tmp, max);
		std::cout << "NewPop=" << tmp << std::endl;
		Env::GetInstance()->gameScreen->population.SetText(tmp);
		delete pop;
		delete max;
		delete tmp;
	}
}

