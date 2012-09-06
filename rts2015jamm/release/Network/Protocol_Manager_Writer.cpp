#include "Protocol_Manager.h"
#include "Sockets_manager.h"
#include "../Env.h"
#include "../Buffer.h"
#include "../Game/Game.h"
#include "../Game/Map.h"
#include "../Game/Player.h"
#include "../Game/Game_Events/Change_Selection.h"
#include "../Game/Game_Events/Create_Units.h"
#include "../Game/Game_Events/Delete_Units.h"
#include "../Game/Game_Events/Orders_Attack.h"
#include "../Game/Game_Events/Orders_Move.h"
#include "../Game/Game_Events/Orders_Follow.h"
#include "../Game/Game_Events/Orders_Hold.h"
#include "../Game/Game_Events/Set_Group.h"
#include "../Game/Game_Events/Use_Group.h"
#include <iostream>

int	Protocol_Manager::__WriteCreateUnits(std::vector<cmd_Create_Units>* cmd, Buffer* buf,
					     std::vector<Socket*>* skts)
{
	Game*	gm;

	std::cout << "WCU\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<cmd_Create_Units>*		str;
	Create_Units*				cu;
	std::vector<cmd_Create_Units>::iterator	it;
	std::vector<cmd_Create_Units>::iterator	end;
	std::vector<dataPosition>::iterator	itDU;
	std::vector<dataPosition>::iterator	endDU;
	std::vector<Socket*>::iterator		itS;
	std::vector<Socket*>::iterator		endS;
	cmd_Create_Units*			model;
	uint16_t				qty;

	str = new std::vector<cmd_Create_Units>(*cmd);
	if (str == NULL)
		return -1;

	cu = new Create_Units(str, gm->currentTick + 1);
	if (cu == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = CREATE_UNIT;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete cu;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		model = &(*it);
		qty = model->units.size();
		if (buf->addstr(&qty, sizeof(uint16_t)) == 0 ||
		    buf->addstr(&(model->id_model), sizeof(uint16_t)) == 0)
		{
			delete str;
			delete cu;
			return -1;
		}
		itDU = model->units.begin();
		endDU = model->units.end();
		while (itDU != endDU)
		{
			if (buf->addstr(&(*itDU), sizeof(dataPosition)) == 0)
			{
				delete str;
				delete cu;
				return -1;
			}
			++itDU;
		}
		++it;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	cmd->clear();
	buf->clear();
	gm->gameEvents.push_back(cu);
	return 0;
}

int	Protocol_Manager::__WriteDeleteUnits(std::vector<dataUnit>* cmd, Buffer* buf,
					     std::vector<Socket*>* skts)
{
	Game*	gm;

	std::cout << "WDU\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<dataUnit>*		str;
	Delete_Units*			du;
	std::vector<dataUnit>::iterator	it;
	std::vector<dataUnit>::iterator	end;
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	uint16_t			qty;

	str = new std::vector<dataUnit>(*cmd);
	if (str == NULL)
		return -1;

	du = new Delete_Units(str, gm->currentTick + 1);
	if (du == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = DELETE_UNIT;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete du;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&(*it), sizeof(dataUnit)) == 0)
		{
			delete str;
			delete du;
			return -1;
		}
		++it;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	cmd->clear();
	buf->clear();
	gm->gameEvents.push_back(du);
	return 0;
}

int	Protocol_Manager::__WriteSelection(std::vector<cmd_Selection>* s, Buffer* buf,
					   std::vector<Socket*>* skts)
{
	Game*	gm;

	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<cmd_Selection>::iterator	it;
	std::vector<cmd_Selection>::iterator	end;
	std::vector<Socket*>::iterator		itS;
	std::vector<Socket*>::iterator		endS;
	std::vector<uint32_t>::iterator		itU;
	std::vector<uint32_t>::iterator		endU;
	uint16_t				qty;

	it = s->begin();
	end = s->end();
	while (it != end)
	{
		if ((*it).sended == false &&
		    (*it).id_units.size() > 0)
		{
			std::cout << "WCS\n";
			this->chunkID = CHANGE_SELECTION;
			qty = (*it).id_units.size();
			if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
			    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
			    buf->addstr(&qty, sizeof(uint16_t)) == 0)
				return -1;
			std::cout << "WCS	qty" << qty << std::endl;
			itU = (*it).id_units.begin();
			endU = (*it).id_units.end();
			while (itU != endU)
			{
				if (buf->addstr(&(*itU), sizeof(uint32_t)) == 0)
					return -1;
				++itU;
			}
			(*it).sended = true;
		}
		else if ((*it).sended == false)
		{
			this->__WriteUseGroup((*it).group, buf);
			(*it).sended = true;
		}
		if ((*it).orders_attack.size() > 0)
			this->__WriteOrdersAttack(&((*it).orders_attack), buf);
		if ((*it).orders_move.size() > 0)
			this->__WriteOrdersMove(&((*it).orders_move), buf);
		if ((*it).orders_follow.size() > 0)
			this->__WriteOrdersFollow(&((*it).orders_follow), buf);
		if ((*it).orders_hold.size() > 0)
			this->__WriteOrdersHold(&((*it).orders_hold), buf);
		if ((*it).set_group.size() > 0)
			this->__WriteSetGroup(&((*it).set_group), buf);
		++it;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	if (s->size() > 1)
	{
		(*s)[0] = s->back();
		s->resize(1);
	}
	buf->clear();
	return 0;
}

int	Protocol_Manager::__WriteOrdersAttack(std::vector<cmd_Orders_Attack>* cmd, Buffer* buf)
{
	Game*	gm;

	std::cout << "WOA\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<cmd_Orders_Attack>*			str;
	Orders_Attack*					oa;
	std::vector<cmd_Orders_Attack>::iterator	it;
	std::vector<cmd_Orders_Attack>::iterator	end;
	std::vector<dataUnit>::iterator			itDU;
	std::vector<dataUnit>::iterator			endDU;
	cmd_Orders_Attack*				attack;
	uint16_t					qty;

	str = new std::vector<cmd_Orders_Attack>(*cmd); // Need to cpy cmd to order
	if (str == NULL)
		return -1;

	oa = new Orders_Attack(str, gm->currentTick + 1);
	if (oa == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = ORDER_ATTACK;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete oa;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		attack = &((*it));
		qty = attack->attackers.size();
		if (buf->addstr(&(attack->target), sizeof(dataUnit)) == 0 ||
		    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		{
			delete str;
			delete oa;
			return -1;
		}
		itDU = attack->attackers.begin();
		endDU = attack->attackers.end();
		while (itDU != endDU)
		{
			if (buf->addstr(&(*itDU), sizeof(dataUnit)) == 0)
			{
				delete str;
				delete oa;
				return -1;
			}
			++itDU;
		}
		++it;
	}
	cmd->clear();
	gm->gameEvents.push_back(oa);
	return 0;
}

int	Protocol_Manager::__WriteOrdersMove(std::vector<cmd_Orders_Move>* cmd, Buffer* buf)
{
	Game*	gm;

	std::cout << "WOM\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<cmd_Orders_Move>*		str;
	Orders_Move*				om;
	std::vector<cmd_Orders_Move>::iterator	it;
	std::vector<cmd_Orders_Move>::iterator	end;
	std::vector<dataUnit>::iterator		itDU;
	std::vector<dataUnit>::iterator		endDU;
	uint16_t				qty;

	str = new std::vector<cmd_Orders_Move>(*cmd); // Need to cpy cmd to order
	if (str == NULL)
		return -1;

	om = new Orders_Move(str, gm->currentTick + 1);
	if (om == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = ORDER_MOVE;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete om;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		qty = (*it).units.size();
		if (buf->addstr(&(*it), sizeof(double) + sizeof(double)) == 0 ||
		    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		{
			delete str;
			delete om;
			return -1;
		}
		itDU = (*it).units.begin();
		endDU = (*it).units.end();
		while (itDU != endDU)
		{
			if (buf->addstr(&(*itDU), sizeof(dataUnit)) == 0)
			{
				delete str;
				delete om;
				return -1;
			}
			++itDU;
		}
		++it;
	}
	cmd->clear();
	gm->gameEvents.push_back(om);
	return 0;
}

int	Protocol_Manager::__WriteOrdersFollow(std::vector<cmd_Orders_Follow>* cmd, Buffer* buf)
{
	Game*	gm;

	std::cout << "WOF\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<cmd_Orders_Follow>*			str;
	Orders_Follow*					of;
	std::vector<cmd_Orders_Follow>::iterator	it;
	std::vector<cmd_Orders_Follow>::iterator	end;
	std::vector<dataUnit>::iterator			itDU;
	std::vector<dataUnit>::iterator			endDU;
	uint16_t					qty;

	str = new std::vector<cmd_Orders_Follow>(*cmd);
	if (str == NULL)
		return -1;

	of = new Orders_Follow(str, gm->currentTick + 1);
	if (of == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = ORDER_FOLLOW;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete of;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		qty = (*it).units.size();
		if (buf->addstr(&(*it), sizeof(dataUnit)) == 0 ||
		    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		{
			delete str;
			delete of;
			return -1;
		}
		itDU = (*it).units.begin();
		endDU = (*it).units.end();
		while (itDU != endDU)
		{
			if (buf->addstr(&(*itDU), sizeof(dataUnit)) == 0)
			{
				delete str;
				delete of;
				return -1;
			}
			++itDU;
		}
		++it;
	}
	cmd->clear();
	gm->gameEvents.push_back(of);
	return 0;
}

int	Protocol_Manager::__WriteOrdersHold(std::vector<dataUnit>* cmd, Buffer* buf)
{
	Game*	gm;

	std::cout << "WOH\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<dataUnit>*			str;
	Orders_Hold*				oh;
	std::vector<dataUnit>::iterator		it;
	std::vector<dataUnit>::iterator		end;
	uint16_t				qty;

	str = new std::vector<dataUnit>(*cmd);
	if (str == NULL)
		return -1;

	oh = new Orders_Hold(str, gm->currentTick + 1);
	if (oh == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = ORDER_HOLD;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete oh;
		return -1;
	}

	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&(*it), sizeof(dataUnit)) == 0)
		{
			delete str;
			delete oh;
			return -1;
		}
		++it;
	}
	cmd->clear();
	gm->gameEvents.push_back(oh);
	return 0;
}

int	Protocol_Manager::__WriteSetGroup(std::vector<uint8_t>* cmd,
					  Buffer* buf)
{
	Game*	gm;

	std::cout << "WSG\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	std::vector<uint8_t>*		str;
	Set_Group*			sg;
	std::vector<uint8_t>::iterator	it;
	std::vector<uint8_t>::iterator	end;
	uint16_t			qty;

	str = new std::vector<uint8_t>(*cmd); // Need to cpy cmd to order
	if (str == NULL)
		return -1;

	sg = new Set_Group(str, gm->myID, gm->currentTick + 1);
	if (sg == NULL)
	{
		delete str;
		return -1;
	}

	this->chunkID = SET_GROUP;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
	{
		delete str;
		delete sg;
		return -1;
	}
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&(*it), sizeof(uint8_t)) == 0)
		{
			delete str;
			delete sg;
			return -1;
		}
		++it;
	}
	cmd->clear();
	gm->gameEvents.push_back(sg);
	return 0;
}

int	Protocol_Manager::__WriteUseGroup(uint8_t idGroup, Buffer* buf)
{
	Game*	gm;

	std::cout << "WUG\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	Use_Group*	ug;

	ug = new Use_Group(idGroup, gm->myID, gm->currentTick + 1);
	if (ug == NULL)
		return -1;
	this->chunkID = USE_GROUP;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&idGroup, sizeof(uint8_t)) == 0)
		return -1;
	gm->gameEvents.push_back(ug);
	return 0;
}

int	Protocol_Manager::__WriteOrderSkill(Protocol* prot, Buffer* buf)
{
	int	i;
	Game*	gm;

	std::cout << "WOS\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	this->chunkID = ORDER_SKILL;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&(prot->skill->qty), sizeof(uint16_t)) == 0)
		return -1;
	i = 0;
	while (i < prot->skill->qty)
	{
		if (buf->addstr(&(prot->skill->id_skill[i]), sizeof(uint16_t)) == 0)
			return -1;
		++i;
	}
	if (buf->addstr(&(prot->skill->x), sizeof(double)) == 0 ||
	    buf->addstr(&(prot->skill->y), sizeof(double)) == 0 ||
	    buf->addstr(&(prot->skill->id_target), sizeof(uint32_t)) == 0)
		return -1;
	return 0;
}

int	Protocol_Manager::__WriteOrderQueueUnit(Protocol* prot, Buffer* buf)
{
	int	i;
	Game*	gm;

	std::cout << "WOQU\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	this->chunkID = ORDER_QUEUE_UNIT;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&(prot->order_queue_unit->qty), sizeof(uint16_t)) == 0)
		return -1;
	i = 0;
	while (i < prot->order_queue_unit->qty)
	{
		if (buf->addstr(&(prot->order_queue_unit->buildings[i].id_unit),
			       sizeof(uint32_t)) == 0 ||
		    buf->addstr(&(prot->order_queue_unit->buildings[i].id_player),
			       sizeof(uint8_t)) == 0 ||
		    buf->addstr(&(prot->order_queue_unit->buildings[i].unused),
			       sizeof(uint8_t) * 3) == 0)
			return -1;
		++i;
	}
	return 0;
}

int	Protocol_Manager::__WriteGameMessage(Protocol* prot, Buffer* buf)
{
	int	i;
	Game*	gm;

	std::cout << "WGM\n";
	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	this->chunkID = GAME_MESSAGE;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&(gm->currentTick), sizeof(uint32_t)) == 0 ||
	    buf->addstr(&(prot->message->qty), sizeof(uint16_t)) == 0)
		return -1;
	i = 0;
	while (i < prot->message->qty)
	{
		if (buf->addstr(&(prot->message->mess[i]), strlen(prot->message->mess[i])) == 0)
			return -1;
		++i;
	}
	return 0;
}

int	Protocol_Manager::__WriteGameEndTick(Buffer* buf, std::vector<Socket*>* p)
{
//	std::cout << "WGET\n";
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;
	Game*				gm;

	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	this->chunkID = END_TICK;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
		return -1;
	gm->players[gm->myID]->SetReadyTick(true);
	it = p->begin();
	end = p->end();
	while (it != end)
	{
		(*it)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++it;
	}
	buf->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyJoin(Buffer* buf, Socket* host)
{
	std::cout << "WLJ\n";
	if (host == NULL)
		return 0;
	this->chunkID = LOBBY_JOIN;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
		return -1;
	if (buf->addstr(Env::GetInstance()->nickname, sizeof(char) * 20) == 0)
		return -1;
	host->send.addstr(buf->buf + buf->buf_pos, buf->size());
	buf->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyJoinAccepted(std::vector<cmd_Lobby_Join_Accepted>* cmd, Buffer* buf)
{
	std::vector<cmd_Lobby_Join_Accepted>::iterator	it;
	std::vector<cmd_Lobby_Join_Accepted>::iterator	end;
	Map*		map;
	char*		name;
	uint16_t	i;
	uint8_t		j;
	uint8_t		ID;
	uint8_t		faction;
	Game*		gm;

	std::cout << "WLJA\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_JOIN_ACC;
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		map = Map::GetInstance();
		gm = Game::GetInstance();
		i = strlen(map->filename);
		if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
		    buf->addstr(&(map->hash), sizeof(uint32_t)) == 0 ||
		    buf->addstr(&((*it).myID), sizeof(uint8_t)) == 0 ||
		    buf->addstr(&i, sizeof(uint16_t)) == 0 ||
		    buf->addstr(map->filename, i) == 0)
			return -1;
		i = 0;
		j = 0;
		while (i < gm->countPlayers)
		{
			if (gm->players[i] != NULL)
				++j;
			++i;
		}
		if (buf->addstr(&j, sizeof(uint8_t)) == 0)
			return -1;
		i = 0;
		while (i < gm->countPlayers)
		{
			if (gm->players[i] != NULL)
			{
				ID = gm->players[i]->GetID();
				faction = gm->players[i]->GetFaction();
				name = gm->players[i]->GetName();
				if (buf->addstr(&ID,
						sizeof(uint8_t)) == 0 ||
				    buf->addstr(&faction,
						sizeof(uint8_t)) == 0 ||
				    buf->addstr(name,
						sizeof(char) * 20) == 0)
					return -1;
			}
			++i;
		}
		(*it).origin->GetSocket()->send.addstr(buf->buf + buf->buf_pos, buf->size());
		buf->clear();
		++it;
	}
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyJoinRefused(std::vector<Socket*>* p,
						  Buffer* buf)
{
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;
	Game*				gm;

	std::cout << "WLJR\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_JOIN_REF;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
		return -1;
	it = p->begin();
	end = p->end();
	while (it != end)
	{
		(*it)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++it;
	}
	buf->clear();
	p->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyNewcomer(std::vector<cmd_Lobby_Newcomer>* cmd,
					       Buffer* buf,
					       std::vector<Socket*>* skts)
{
	std::vector<cmd_Lobby_Newcomer>::iterator	it;
	std::vector<cmd_Lobby_Newcomer>::iterator	end;
	std::vector<Socket*>::iterator			itS;
	std::vector<Socket*>::iterator			endS;
	uint16_t					qty;
	Game*						gm;

	std::cout << "WLN\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_NEWCOMER;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		return -1;
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&((*it).ID), sizeof(uint8_t)) == 0 ||
		    buf->addstr(&((*it).faction), sizeof(uint8_t)) == 0 ||
		    buf->addstr(&((*it).name), sizeof(char) * 20) == 0)
			return -1;
		++it;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	buf->clear();
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyUpdFaction(std::vector<cmd_Lobby_Upd_Faction>* cmd,
						 Buffer* buf,
						 std::vector<Socket*>* skts)
{
	std::vector<cmd_Lobby_Upd_Faction>::iterator	it;
	std::vector<cmd_Lobby_Upd_Faction>::iterator	end;
	std::vector<Socket*>::iterator			itS;
	std::vector<Socket*>::iterator			endS;
	uint16_t					qty;
	Game*						gm;

	std::cout << "WLUF\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_UPD_FACTION;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		return -1;
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&((*it).ID), sizeof(uint8_t)) == 0 ||
		    buf->addstr(&((*it).faction), sizeof(uint8_t)) == 0)
			return -1;
		++it;
	}
	if (gm->AmIHost == true)
	{
		itS = skts->begin();
		endS = skts->end();
		while (itS != endS)
		{
			(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
			++itS;
		}
	}
	else
		Sockets_manager::GetInstance()
			->GetHostSocket()
			->send.addstr(buf->buf + buf->buf_pos, buf->size());
	buf->clear();
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyUpdID(std::vector<cmd_Lobby_Upd_ID>* cmd,
						 Buffer* buf,
						 std::vector<Socket*>* skts)
{
	std::vector<cmd_Lobby_Upd_ID>::iterator	it;
	std::vector<cmd_Lobby_Upd_ID>::iterator	end;
	std::vector<Socket*>::iterator		itS;
	std::vector<Socket*>::iterator		endS;
	uint16_t				qty;
	Game*				gm;

	std::cout << "WLUI\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_UPD_ID;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		return -1;
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		if (buf->addstr(&((*it).ID), sizeof(uint8_t)) == 0 ||
		    buf->addstr(&((*it).newID), sizeof(uint8_t)) == 0)
			return -1;
		++it;
	}
	if (Game::GetInstance()->AmIHost == true)
	{
		itS = skts->begin();
		endS = skts->end();
		while (itS != endS)
		{
			(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
			++itS;
		}
	}
	else
		Sockets_manager::GetInstance()
			->GetHostSocket()
			->send.addstr(buf->buf + buf->buf_pos, buf->size());
	buf->clear();
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyMessage(std::vector<char*>* cmd,
					      Buffer* buf,
					      std::vector<Socket*>* skts)
{
	std::vector<char*>::iterator	it;
	std::vector<char*>::iterator	end;
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	uint16_t			qty;
	Game*				gm;

	std::cout << "WLM\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = LOBBY_UPD_ID;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		return -1;
	it = cmd->begin();
	end = cmd->end();
	while (it != end)
	{
		qty = strlen((*it));
		if (buf->addstr(&qty, sizeof(uint16_t)) == 0 ||
		    buf->addstr(*it, sizeof(char) * qty) == 0)
			return -1;
		++it;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	buf->clear();
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteLobbyLeave(std::vector<uint8_t>* cmd,
					    Buffer* buf,
					    std::vector<Socket*>* skts)
{
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	std::vector<uint8_t>::iterator	itID;
	std::vector<uint8_t>::iterator	endID;
	Sockets_manager*		sm;
	uint16_t			qty;
	Game*				gm;

	std::cout << "WLL\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
	{
		cmd->clear();
		return 0;
	}
	this->chunkID = LOBBY_LEAVE;
	qty = cmd->size();
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0 ||
	    buf->addstr(&qty, sizeof(uint16_t)) == 0)
		return -1;
	sm = Sockets_manager::GetInstance();
	if (gm->AmIHost == false)
	{
		itID = cmd->begin();
		if (buf->addstr(&(*itID), sizeof(uint8_t)) == 0)
			return -1;
		if (sm->GetHostSocket() != NULL)
			sm->GetHostSocket()->send.addstr(buf->buf + buf->buf_pos, buf->size());
		cmd->clear();
		return 0;
	}
	itID = cmd->begin();
	endID = cmd->end();
	while (itID != endID)
	{
		if (buf->addstr(&(*itID), sizeof(uint8_t)) == 0)
			return -1;
		++itID;
	}
	itS = skts->begin();
	endS = skts->end();
	while (itS != endS)
	{
		(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
		++itS;
	}
	buf->clear();
	cmd->clear();
	return 0;
}

int	Protocol_Manager::__WriteStartListen(Buffer* buf,
					     std::vector<Socket*>* skts)
{
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	Game*				gm;

	std::cout << "WSL\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = START_LISTEN;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
		return -1;
	if (gm->AmIHost == true)
	{
		itS = skts->begin();
		endS = skts->end();
		while (itS != endS)
		{
			(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
			++itS;
		}
	}
	else if (Sockets_manager::GetInstance()->GetListenSocket() != NULL)
	{
		Sockets_manager::GetInstance()
			->GetHostSocket()
			->send.addstr(buf->buf + buf->buf_pos, buf->size());
	}
	buf->clear();
	return 0;
}

int	Protocol_Manager::__WriteConnectEachOthers(Buffer* buf,
						   std::vector<Socket*>* skts)
{
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	Game*				gm;
	uint8_t				i;
	uint8_t				countPlayers;
	uint8_t				countHostingPlayers;
	char*				ip;

	std::cout << "WCEO\n";
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	this->chunkID = CONNECT_EACH_OTHERS;
	if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
		return -1;
	if (gm->AmIHost == false)
	{
		std::cout << "Connected to all, notice host...\n";
		Sockets_manager::GetInstance()
			->GetHostSocket()
			->send.addstr(buf->buf + buf->buf_pos, buf->size());
		buf->clear();
		std::cout << "S=" << Sockets_manager::GetInstance()->GetSockets()->size() << std::endl;
		return 0;
	}
	countPlayers = 0;
	countHostingPlayers = 0;
	i = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL && i != gm->myID)
		{
			if (gm->players[i]->GetHosting() == true)
			{
				ip = gm->players[i]->GetSocket()->ip;
				std::cout << "IP" << ip << "\n";
				if (buf->addstr(&i, sizeof(uint8_t)) == 0 ||
				    buf->addstr(ip, sizeof(char) * 16) == 0)
					return -1;
				++countHostingPlayers;
			}
			++countPlayers;
		}
		++i;
	}
	if (countPlayers == countHostingPlayers)
	{
		itS = skts->begin();
		endS = skts->end();
		while (itS != endS)
		{
			(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
			++itS;
		}
	}
	buf->clear();
	return 0;
}

int	Protocol_Manager::__WriteStartGame(Buffer* buf,
					   std::vector<Socket*>* skts)
{
	std::vector<Socket*>::iterator	itS;
	std::vector<Socket*>::iterator	endS;
	Game*				gm;
	uint8_t				i;
	uint8_t				countPlayers;
	uint8_t				IP;

	std::cout << "WSG" << std::endl;
	gm = Game::GetInstance();
	if (gm->AmIHost == false)
		return 0;
	i = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL)
			++countPlayers;
		if (countPlayers > 1)
			break ;
		++i;
	}
	if (countPlayers > 1)
	{
		gm->InitGame();
		this->chunkID = START_GAME;
		if (buf->addstr(&(this->chunkID), sizeof(uint16_t)) == 0)
			return -1;
		i = 0;
		while (i < gm->countPlayers)
		{
			if (gm->players[i] != NULL)
			{
				IP = gm->players[i]->GetInitialPos();
				if (buf->addstr(&IP, sizeof(uint8_t)) == 0)
					return -1;
			}
			++i;
		}
		itS = skts->begin();
		endS = skts->end();
		while (itS != endS)
		{
			(*itS)->send.addstr(buf->buf + buf->buf_pos, buf->size());
			++itS;
		}
		gm->StartGame();
	}
	buf->clear();
	return 0;
}
