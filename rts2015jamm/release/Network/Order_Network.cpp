#include "Order_Network.h"
#include "Protocol_Manager.h"
#include "../Game/Player.h"
#include "../Game/Unit.h"
#include "../Game/Game.h"

int	Protocol::AddCmdGameSelection(Player* p)
{
	cmd_Selection			sel;
	std::list<Unit*>*		units;
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;
	int				i;

	units = p->GetSelection();
	sel.id_player = p->GetID();
/*	sel->id_units = new uint32_t[units->size()];
	if (sel->id_units == NULL)
	{
		delete sel;
		return -1;
	}
*/	it = units->begin();
	end = units->end();
	i = 0;
	while (it != end)
	{
		sel.id_units.push_back((*it)->GetToken());
		++it;
	}
	sel.sended = false;
	this->selection.push_back(sel);
	return 0;
}

void	Protocol::AddCmdGameCreateUnit(uint16_t id_model, double x, double y, uint8_t id_player)
{
	dataPosition				u;
	std::vector<cmd_Create_Units>::iterator	it;
	std::vector<cmd_Create_Units>::iterator	end;

	it = this->create_units.begin();
	end = this->create_units.end();
	u.x = x;
	u.y = y;
	u.id_player = id_player;
	while (it != end)
	{
		if ((*it).id_model == id_model)
		{
			(*it).units.push_back(u);
			return ;
		}
		++it;
	}
	cmd_Create_Units	m(id_model);

	m.units.push_back(u);
	this->create_units.push_back(m);
}

void	Protocol::AddCmdGameDeleteUnit(uint32_t id_unit, uint8_t id_player)
{
	dataUnit	u;

	u.id_unit = id_unit;
	u.id_player = id_player;
	this->delete_units.push_back(u);
}

void	Protocol::AddCmdGameOrderAttack(uint32_t u_target, uint8_t p_target,
				    uint32_t u_attacker, uint8_t p_attacker)
{
	dataUnit					u;
	cmd_Orders_Attack				m;
	std::vector<cmd_Orders_Attack>::iterator	it;
	std::vector<cmd_Orders_Attack>::iterator	end;
	cmd_Selection*					sel;

/*	if (this->selection.size() == 0)
	{
		cmd_Selection	s;

		s.group = 0;
		s.id_player = p_attacker;
		this->selection.push_back(s);
		sel = &(this->selection.back());
	}
	else*/
		sel = &(this->selection.back());
	it = sel->orders_attack.begin();
	end = sel->orders_attack.end();

	u.id_unit = u_attacker;
	u.id_player = p_attacker;
	while (it != end)
	{
		if ((*it).target.id_unit == u_target &&
		    (*it).target.id_player == p_target)
		{
			(*it).attackers.push_back(u);
			return ;
		}
		++it;
	}
	m.target.id_unit = u_target;
	m.target.id_player = p_target;
	m.attackers.push_back(u);
	sel->orders_attack.push_back(m);
}

int	Protocol::AddCmdGameOrderMove(double x, double y, uint32_t id_unit, uint8_t id_player)
{
	dataUnit				u;
	cmd_Orders_Move				m;
	std::vector<cmd_Orders_Move>::iterator	it;
	std::vector<cmd_Orders_Move>::iterator	end;
	cmd_Selection*				sel;

	sel = &(this->selection.back());
	it = sel->orders_move.begin();
	end = sel->orders_move.end();

	u.id_unit = id_unit;
	u.id_player = id_player;
	while (it != end)
	{
		if ((*it).x == x && (*it).y == y)
		{
			(*it).units.push_back(u);
			return 0;
		}
		++it;
	}
	m.x = x;
	m.y = y;
	m.units.push_back(u);
	sel->orders_move.push_back(m);
	return 0;
}

int	Protocol::AddCmdGameOrderFollow(uint32_t u_target, uint8_t p_target,
					uint32_t u_follower, uint8_t p_follower)
{
	dataUnit					u;
	cmd_Orders_Follow				m;
	std::vector<cmd_Orders_Follow>::iterator	it;
	std::vector<cmd_Orders_Follow>::iterator	end;
	cmd_Selection*					sel;

	sel = &(this->selection.back());
	// CREATE FCT TO MANAGE SELECTION
	it = sel->orders_follow.begin();
	end = sel->orders_follow.end();

	u.id_unit = u_follower;
	u.id_player = p_follower;
	while (it != end)
	{
		if ((*it).target.id_unit == u_target &&
		    (*it).target.id_player == p_target)
		{
			(*it).units.push_back(u);
			return 0;
		}
		++it;
	}
	m.target.id_unit = u_target;
	m.target.id_player = p_target;
	m.units.push_back(u);
	sel->orders_follow.push_back(m);
	return 0;
}

int	Protocol::AddCmdGameOrderHold(uint32_t u_hold, uint8_t p_hold)
{
	dataUnit					u;
	cmd_Selection*					sel;

	sel = &(this->selection.back());
	u.id_unit = u_hold;
	u.id_player = p_hold;
	sel->orders_hold.push_back(u);
	return 0;
}

void	Protocol::AddCmdGameSetGroup(uint8_t id_grp)
{
	if (Game::GetInstance()->GetMe()->GetSelection()->size() > 0)
		this->selection.back().set_group.push_back(id_grp);
}

void	Protocol::AddCmdGameUseGroup(uint8_t id_grp, uint8_t id_player)
{
	cmd_Selection	newSel;

	newSel.group = id_grp;
	newSel.id_player = id_player;
	newSel.sended = false;
	this->selection.push_back(newSel);
}

void	Protocol::AddCmdGameSkill(double x, double y, uint32_t u_target, uint8_t p_target, uint16_t id_skill)
{
//Faudra la faire quand le systeme de skill sera vraiment defini...
	dataUnit	u;

	if (this->skill == NULL)
		this->skill = new cmd_Skill;
	++this->skill->qty;
	this->skill->x = x;
	this->skill->y = y;
(void)x;
(void)y;
(void)u_target;
(void)p_target;
(void)id_skill;
}

void	Protocol::AddCmdGameQueue(uint16_t id_unit, uint8_t id_player)
{
	dataUnit	u;

	if (this->order_queue_unit == NULL)
		this->order_queue_unit = new cmd_Queue_unit;
	++this->order_queue_unit->qty;
	u.id_unit = id_unit;
	u.id_player = id_player;
	this->order_queue_unit->buildings.push_back(u);
}

void	Protocol::AddCmdGameMessage(char* mess)
{
	if (this->message == NULL)
		this->message = new cmd_Message;
	++this->message->qty;
	this->message->mess.push_back(mess);
}

int	Protocol::AddCmdGameEndTick(void)
{
	this->endTick = true;
	return 0;
}

int	Protocol::AddCmdLobbyJoin(void)
{
	this->lobbyJoin = true;
	return 0;
}

int	Protocol::AddCmdLobbyJoinAccepted(uint8_t myID, Player* p)
{
	cmd_Lobby_Join_Accepted	cmd;

	cmd.myID = myID;
	cmd.origin = p;
	this->lobbyJoinAccepted.push_back(cmd);
	return 0;
}

int	Protocol::AddCmdLobbyJoinRefused(Socket* p)
{
	this->lobbyJoinRefused.push_back(p);
	return 0;
}

int	Protocol::AddCmdLobbyNewcomer(uint8_t ID, uint8_t faction, char* name)
{
	cmd_Lobby_Newcomer	cmd;

	cmd.ID = ID;
	cmd.faction = faction;
	strncpy(cmd.name, name, 20);
	this->lobbyNewcomer.push_back(cmd);
	return 0;
}

int	Protocol::AddCmdLobbyUpdFaction(uint8_t ID, uint8_t faction)
{
	cmd_Lobby_Upd_Faction	cmd;
	Game*			g;

	g = Game::GetInstance();
	if (g->AmIHost == true)
	{
		if (g->UpdatePlayerFaction(ID, faction) == 0)
			return 0;
	}
	cmd.ID = ID;
	cmd.faction = faction;
	this->lobbyUpdFaction.push_back(cmd);
	return 0;
}

int	Protocol::AddCmdLobbyUpdID(uint8_t ID, uint8_t newID)
{
	cmd_Lobby_Upd_ID	cmd;
	Game*			g;

	g = Game::GetInstance();
	if (g->AmIHost == true)
	{
		if (g->UpdatePlayerID(ID, newID) == 0)
			return 0;
	}
	cmd.ID = ID;
	cmd.newID = newID;
	this->lobbyUpdID.push_back(cmd);
	return 0;
}

int	Protocol::AddCmdLobbyMessage(char* m)
{
	this->lobbyMessage.push_back(m);
	return 0;
}

int	Protocol::AddCmdLobbyLeave(uint8_t ID)
{
	this->lobbyLeave.push_back(ID);
	return 0;
}

int	Protocol::AddCmdStartListen(void)
{
	this->startListen = true;
	return 0;
}

int	Protocol::AddCmdConnectEachOthers(void)
{
	this->connectEachOthers = true;
	return 0;
}

int	Protocol::AddCmdStartGame(void)
{
	this->startGame = true;
	return 0;
}

