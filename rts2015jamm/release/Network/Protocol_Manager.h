#ifndef __PROTOCOL_MANAGER_H__
#define __PROTOCOL_MANAGER_H__

#include <vector>
#include "../Singleton.hpp"
#include "../ID_glibrary.h"
#include "Socket.h"
#include "Order_Network.h"
#include "../Buffer.h"

/*
ID determine la position dans la liste d'emplacement dispo
Deroulement pre-ingame PDV Host
LoadMap->Host
Chunk Lobby Join
{
	if (SLOT)
		myID = Add to list;
		set player->socket;
		Send Chunk Lobby Accept (
					HASH,
					myID,
					length,
					NAME_MAP,
					length,
					P1._ID, P1._FACTION, length or ALREADY 20,
						   P1._NAME,
					...)
		Send all Chunk Lobby Newcomer (ID, FACTION, 20, NAME)
	else
		Chunk Lobby Refuse
}
Chunk Lobby Upd faction (ID, NEW FACTION)
Chunk Lobby Upd ID (ID, NEW ID)
{
	if (CHECK)
	{
		Sendall same Chunk
		return Chunk Lobby Upd Accepted
	}
	return Chunk Lobby Upd Refused
}
Chunk Lobby Leave (ID)
{
	Sendall Chunk Lobby Leave (ID)
}

Deroulement pre-ingame PDV Join
Send host Chunk Lobby Join
Chunk Lobby Accept
{
	if (LoadMap)
	{
		if (hash ==)
		{
			//END
			return;
		}
	}
	Chunk Lobby Leave
}
Send host Chunk Lobby Upd faction (ID, NEW FACTION)
Send host Chunk Lobby Upd ID (ID, NEW ID)
{
	Add to a request queue
	wait for the answer
}
Chunk Lobby Upd Accepted
Chunk Lobby Upd Refused
{
	Execute ou pas la request queue
}
Send host Chunk Lobby Leave
Chunk Start Game
{
HERE WE GO!
}
*/

/*
*	1XXX = EXCHANGE HOST/CLIENT
*	2XXX = INGAME
*	9XXX = UNCATEGORIZED
*/
enum	chunkID
{
	LOBBY_JOIN	= 0x1000,
	LOBBY_JOIN_ACC	= 0x1010,
	LOBBY_JOIN_REF	= 0x1020,
	LOBBY_NEWCOMER	= 0x1030,
	LOBBY_UPD_FACTION=0x1120,
	LOBBY_UPD_ID	= 0x1130,
	LOBBY_MESSAGE	= 0x1200,
	LOBBY_LEAVE	= 0x1900,
	START_LISTEN	= 0x1950,
	CONNECT_EACH_OTHERS= 0x1960,
	INIT_SETTINGS	= 0x1970,

	START_GAME	= 0x2000,
	CREATE_UNIT	= 0x2100,
	DELETE_UNIT	= 0x2110,
	CHANGE_SELECTION= 0x2200,
	ORDER_ATTACK	= 0x2210,
	ORDER_MOVE	= 0x2220,
	ORDER_FOLLOW	= 0x2225,
	ORDER_HOLD	= 0x2250,
	ORDER_SKILL	= 0x2230,
	SET_GROUP	= 0x2240,
	USE_GROUP	= 0x2245,
//	BUILD_BUILDING	= 0x2300,
//	BUILD_UNIT_QUEUE= 0x2310,
	GAME_MESSAGE	= 0x2400,
	ORDER_QUEUE_UNIT= 0x2500,
	END_TICK	= 0x2998,
	LEAVE_GAME	= 0x2999,

	END_TRANSMISSION= 0x9000
};

class	Buffer;

class	Protocol_Manager : public Singleton<Protocol_Manager>
{
	friend class	Singleton<Protocol_Manager>;
 public:
	Protocol_Manager();
	~Protocol_Manager();

	int	Read(int fd);
	int	Write(std::vector<Socket*>* sockets);
	int	Parse(Buffer*);

 private:
	int	__ExecChunk(Buffer*);

	int	__ReadCreateUnits(Buffer*);
	int	__ReadDeleteUnits(Buffer*);
	int	__ReadChangeSelection(Buffer*);
	int	__ReadOrdersAttack(Buffer*);
	int	__ReadOrdersMove(Buffer*);
	int	__ReadOrdersFollow(Buffer*);
	int	__ReadOrdersHold(Buffer*);
	int	__ReadSetGroup(Buffer*);
	int	__ReadUseGroup(Buffer*);
	int	__ReadOrderSkill(Buffer*);
	int	__ReadOrderQueueUnit(Buffer*);
//	int	__ReadGameMessage(Buffer*);
	int	__ReadGameEndTick(void);

	int	__ReadLobbyJoin(Buffer*);
	int	__ReadLobbyJoinAccepted(Buffer*);
	int	__ReadLobbyJoinRefused(void);
	int	__ReadLobbyNewcomer(Buffer*);
	int	__ReadLobbyUpdFaction(Buffer*);
	int	__ReadLobbyUpdID(Buffer*);
	int	__ReadLobbyMessage(Buffer*);
	int	__ReadLobbyLeave(Buffer*);
	int	__ReadStartListen(void);
	int	__ReadConnectEachOthers(Buffer*);
	int	__ReadStartGame(Buffer*);

	int	__WriteCreateUnits(std::vector<cmd_Create_Units>*, Buffer*,
				   std::vector<Socket*>* sockets);
	int	__WriteDeleteUnits(std::vector<dataUnit>*, Buffer*,
				   std::vector<Socket*>* sockets);
	int	__WriteSelection(std::vector<cmd_Selection>*, Buffer*,
				 std::vector<Socket*>* sockets);
	int	__WriteOrdersAttack(std::vector<cmd_Orders_Attack>*, Buffer*);
	int	__WriteOrdersMove(std::vector<cmd_Orders_Move>*, Buffer*);
	int	__WriteOrdersFollow(std::vector<cmd_Orders_Follow>*, Buffer*);
	int	__WriteOrdersHold(std::vector<dataUnit>*, Buffer*);
	int	__WriteSetGroup(std::vector<uint8_t>*, Buffer*);
	int	__WriteUseGroup(uint8_t idGroup, Buffer*);
	int	__WriteOrderSkill(Protocol*, Buffer*);
	int	__WriteOrderQueueUnit(Protocol*, Buffer*);
	int	__WriteGameMessage(Protocol*, Buffer*);
	int	__WriteGameEndTick(Buffer*,
				   std::vector<Socket*>* sockets);

	int	__WriteLobbyJoin(Buffer*, Socket*);
	int	__WriteLobbyJoinAccepted(std::vector<cmd_Lobby_Join_Accepted>*, Buffer*);
	int	__WriteLobbyJoinRefused(std::vector<Socket*>*, Buffer*);
	int	__WriteLobbyNewcomer(std::vector<cmd_Lobby_Newcomer>*,
				     Buffer*,
				     std::vector<Socket*>* sockets);
	int	__WriteLobbyUpdFaction(std::vector<cmd_Lobby_Upd_Faction>*,
				       Buffer*,
				       std::vector<Socket*>* sockets);
	int	__WriteLobbyUpdID(std::vector<cmd_Lobby_Upd_ID>*,
				  Buffer*,
				  std::vector<Socket*>* sockets);
	int	__WriteLobbyMessage(std::vector<char*>*,
				    Buffer*,
				    std::vector<Socket*>* sockets);
	int	__WriteLobbyLeave(std::vector<uint8_t>*,
				  Buffer*,
				  std::vector<Socket*>* sockets);
	int	__WriteStartListen(Buffer*,
				   std::vector<Socket*>* sockets);
	int	__WriteConnectEachOthers(Buffer*,
					 std::vector<Socket*>* sockets);
	int	__WriteStartGame(Buffer*,
				 std::vector<Socket*>* sockets);

 public:
	Protocol	prot;
	Socket*		workingSocket;

 private:
	std::vector<uint16_t>::iterator	it_16;
	std::vector<uint32_t>::iterator	it_32;
	uint16_t			tmp_16;
	uint32_t			tmp_32;
	void*				empty;
	uint16_t			chunkID;
	uint16_t			qty;
	bool				incompleteChunk;
	uint8_t				step;
	Buffer				buf;
};

#endif
