#include "Protocol_Manager.h"
#include "Sockets_manager.h"
#include "../Buffer.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

Protocol_Manager::Protocol_Manager() : empty(NULL),
				       incompleteChunk(false),
				       step(0),
				       buf(1024, true)
{
}

Protocol_Manager::~Protocol_Manager()
{
}

int	Protocol_Manager::Read(int fd)
{
	int		n;
	int		ret;

	n = this->buf.read(fd, 1024);
	if (n <= 0)
		return -1;
	while (n >= 0)
	{
		ret = this->Parse(&(this->buf));
		if (ret == 0)
			return 0;
		n = this->buf.read(fd, 1024);
		if (n <= 0)
			return -1;
	}
	return -1;
}

int	Protocol_Manager::Parse(Buffer* b)
{
	int		ret;

	if (this->incompleteChunk == true)
	{
		ret = this->__ExecChunk(b);
		if (ret == -1)
		{
			this->incompleteChunk = true;
			return -1;
		}
		else if (ret == 0)
		{
			return 0;
		}
	}
	this->incompleteChunk = false;
	while (this->empty == NULL)
	{
		if (b->size() >= 2)
		{
			b->getstr(&(this->workingSocket->chunkID), sizeof(uint16_t));
			this->step = 0;
			std::cout << "Size()=" << b->size() << "	ID" << std::hex << this->workingSocket->chunkID << std::dec << std::endl;
			ret = this->__ExecChunk(b);
			if (ret == -1)
			{
				this->incompleteChunk = true;
				return -1;
			}
			else if (ret == 0)
			{
				return 0;
			}
		}
		else
		{
			this->incompleteChunk = true;
			return -1;
		}
	}
	return -1;
}

int	Protocol_Manager::__ExecChunk(Buffer* b)
{
	if (this->workingSocket->chunkID == CREATE_UNIT)
	{
		if (this->__ReadCreateUnits(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == DELETE_UNIT)
	{
		if (this->__ReadDeleteUnits(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == CHANGE_SELECTION)
	{
		if (this->__ReadChangeSelection(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == ORDER_ATTACK)
	{
		if (this->__ReadOrdersAttack(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == ORDER_MOVE)
	{
		if (this->__ReadOrdersMove(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == ORDER_FOLLOW)
	{
		if (this->__ReadOrdersFollow(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == ORDER_HOLD)
	{
		if (this->__ReadOrdersHold(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == SET_GROUP)
	{
		if (this->__ReadSetGroup(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == USE_GROUP)
	{
		if (this->__ReadUseGroup(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == ORDER_SKILL)
	{
		if (this->__ReadOrderSkill(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == END_TICK)
	{
		if (this->__ReadGameEndTick() == -1)
			return -1;
	}

	else if (this->workingSocket->chunkID == LOBBY_JOIN)
	{
		if (this->__ReadLobbyJoin(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_JOIN_ACC)
	{
		if (this->__ReadLobbyJoinAccepted(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_JOIN_REF)
	{
		if (this->__ReadLobbyJoinRefused() == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_NEWCOMER)
	{
		if (this->__ReadLobbyNewcomer(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_UPD_FACTION)
	{
		if (this->__ReadLobbyUpdFaction(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_UPD_ID)
	{
		if (this->__ReadLobbyUpdID(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_MESSAGE)
	{
		if (this->__ReadLobbyMessage(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == LOBBY_LEAVE)
	{
		if (this->__ReadLobbyLeave(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == START_LISTEN)
	{
		if (this->__ReadStartListen() == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == CONNECT_EACH_OTHERS)
	{
		if (this->__ReadConnectEachOthers(b) == -1)
			return -1;
	}
	else if (this->workingSocket->chunkID == START_GAME)
	{
		if (this->__ReadStartGame(b) == -1)
			return -1;
	}
	/*
	else if (this->chunkID == GAME_MESSAGE)
	{
		static cmd_Message	str;
	}
	*/
	else if (this->workingSocket->chunkID == END_TRANSMISSION)
	{
		return 0;
	}
	return 1;
}

int	Protocol_Manager::Write(std::vector<Socket*>* skts)
{
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;

	if (this->__WriteSelection(&(this->prot.selection), &(this->buf), skts) == -1)
		return -1;
	if (this->prot.create_units.size() > 0)
	{
		if (this->__WriteCreateUnits(&(this->prot.create_units), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.delete_units.size() > 0)
	{
		if (this->__WriteDeleteUnits(&(this->prot.delete_units), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.skill != NULL)
	{
		if (this->__WriteOrderSkill(&this->prot, &(this->buf)) == -1)
			return -1;
	}
	if (this->prot.order_queue_unit != NULL)
	{
		if (this->__WriteOrderQueueUnit(&this->prot, &(this->buf)) == -1)
			return -1;
	}
	if (this->prot.message != NULL)
	{
		if (this->__WriteGameMessage(&this->prot, &(this->buf)) == -1)
			return -1;
	}
	if (this->prot.endTick == true)
	{
		if (this->__WriteGameEndTick(&(this->buf), skts) == -1)
			return -1;
		this->prot.endTick = false;
	}

	if (this->prot.lobbyJoin == true)
	{
		if (this->__WriteLobbyJoin(&(this->buf), Sockets_manager::GetInstance()->GetHostSocket()) == -1)
			return -1;
		this->prot.lobbyJoin = false;
	}
	if (this->prot.lobbyJoinAccepted.size() > 0)
	{
		if (this->__WriteLobbyJoinAccepted(&(this->prot.lobbyJoinAccepted), &(this->buf)) == -1)
			return -1;
	}
	if (this->prot.lobbyJoinRefused.size() > 0)
	{
		if (this->__WriteLobbyJoinRefused(&(this->prot.lobbyJoinRefused), &(this->buf)) == -1)
			return -1;
	}
	if (this->prot.lobbyNewcomer.size() > 0)
	{
		if (this->__WriteLobbyNewcomer(&(this->prot.lobbyNewcomer), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.lobbyUpdFaction.size() > 0)
	{
		if (this->__WriteLobbyUpdFaction(&(this->prot.lobbyUpdFaction), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.lobbyUpdID.size() > 0)
	{
		if (this->__WriteLobbyUpdID(&(this->prot.lobbyUpdID), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.lobbyMessage.size() > 0)
	{
		if (this->__WriteLobbyMessage(&(this->prot.lobbyMessage), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.lobbyLeave.size() > 0)
	{
		if (this->__WriteLobbyLeave(&(this->prot.lobbyLeave), &(this->buf), skts) == -1)
			return -1;
	}
	if (this->prot.startListen == true)
	{
		if (this->__WriteStartListen(&(this->buf), skts) == -1)
			return -1;
		this->prot.startListen = false;
	}
	if (this->prot.connectEachOthers == true)
	{
		if (this->__WriteConnectEachOthers(&(this->buf), skts) == -1)
			return -1;
		this->prot.connectEachOthers = false;
	}
	if (this->prot.startGame == true)
	{
		if (this->__WriteStartGame(&(this->buf), skts) == -1)
			return -1;
		this->prot.startGame = false;
	}
	this->chunkID = END_TRANSMISSION;
	it = skts->begin();
	end = skts->end();
	while (it != end)
	{
		if ((*it)->send.size() > 0)
		{
			(*it)->send.addstr(&(this->chunkID), sizeof(uint16_t));
		}
		++it;
	}
	return 0;
}
