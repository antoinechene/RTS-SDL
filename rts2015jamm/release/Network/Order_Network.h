#ifndef __ORDER_NETWORK_H__
#define __ORDER_NETWORK_H__

#include <stdint.h>
#include <vector>

class	Unit_Model;
class	Unit;
class	Player;
class	Skill;
class	Socket;

struct	dataUnit
{
	dataUnit() : id_unit(-1), id_player(-1)
	{
	}

	uint32_t	id_unit;
	uint8_t		id_player;
	uint8_t		unused[3];
};

struct	dataPosition
{
	double	x;
	double	y;
	uint8_t	id_player;
	uint8_t	unused[3];
};

struct cmd_Create_Units
{
	cmd_Create_Units(uint16_t id = 0) : id_model(id)
	{
	}

	uint16_t			id_model;
	std::vector<dataPosition>	units;
};

struct cmd_Orders_Attack
{
	dataUnit		target;
	std::vector<dataUnit>	attackers;
};

struct cmd_Orders_Move
{
	double			x;
	double			y;
	std::vector<dataUnit>	units;
};

struct cmd_Orders_Follow
{
	dataUnit		target;
	std::vector<dataUnit>	units;
};

struct cmd_Set_Group
{
	uint8_t	id_group;
	uint8_t	id_player;
	uint8_t	unused[2];
};

struct cmd_Use_group
{
	uint8_t	id_group;
	uint8_t	id_player;
	uint8_t	unused[2];
};

struct cmd_Skill
{
	cmd_Skill() : x(-1), y(-1), qty(0)
	{
	}

	double			x;
	double			y;
	dataUnit		id_target;
	uint16_t		qty;
	std::vector<uint16_t>	id_skill;
};

struct	cmd_Queue_unit
{
	cmd_Queue_unit() : qty()
	{
	}

	uint16_t		qty;
	std::vector<dataUnit>	buildings;

};

struct cmd_Message
{
	cmd_Message() : qty(0)
	{
	}

	uint16_t		qty;
	std::vector<char*>	mess;
};

struct	cmd_Selection
{
	std::vector<cmd_Orders_Attack>	orders_attack;
	std::vector<cmd_Orders_Move>	orders_move;
	std::vector<cmd_Orders_Follow>	orders_follow;
	std::vector<dataUnit>		orders_hold;
	std::vector<uint8_t>		set_group;
	std::vector<uint32_t>		id_units;
	uint8_t				group;
	uint8_t				id_player;
	bool				sended;
};

struct	cmd_Lobby_Join_Accepted
{
	uint8_t		myID;
	Player*		origin;
};

struct	cmd_Lobby_Newcomer
{
	uint8_t	ID;
	uint8_t	faction;
	char	name[22];
};

struct	cmd_Lobby_Upd_Faction
{
	uint8_t	ID;
	uint8_t	faction;
};

struct	cmd_Lobby_Upd_ID
{
	uint8_t	ID;
	uint8_t	newID;
};

struct	cmd_Lobby_Message
{
	char*	message;
};

struct	cmd_Lobby_Leave
{
};

class	Protocol
{
 public:
	Protocol()/* : create(NULL), delete_unit(NULL),
			order_attack(NULL), order_move(NULL),
			set_group(NULL), use_group(NULL),
			*/ :
			skill(NULL),
			order_queue_unit(NULL),
			message(NULL),
			endTick(false),
			lobbyJoin(false),
			startListen(false),
			connectEachOthers(false),
			startGame(false)
	{
	}
	~Protocol()
	{
	}

	// all = all except myself
	// Send all
	std::vector<cmd_Selection>	selection;
	std::vector<cmd_Create_Units>	create_units;
	std::vector<dataUnit>		delete_units;
	cmd_Skill*			skill;
	cmd_Queue_unit*			order_queue_unit;
	// Send particular
	cmd_Message*			message;
	bool				endTick;

	// Send host
	bool					lobbyJoin;
	// Send join
	std::vector<cmd_Lobby_Join_Accepted>	lobbyJoinAccepted;
	std::vector<Socket*>			lobbyJoinRefused;
	// Send all -origin
	std::vector<cmd_Lobby_Newcomer>		lobbyNewcomer;
	// Send all -origin/host
	std::vector<cmd_Lobby_Upd_Faction>	lobbyUpdFaction;
	std::vector<cmd_Lobby_Upd_ID>		lobbyUpdID;
	// Send all
	std::vector<char*>			lobbyMessage;
	// Send all
	std::vector<uint8_t>			lobbyLeave;
	// Send all
	bool					startListen;
	bool					connectEachOthers;
	bool					startGame;

	int	AddCmdGameSelection(Player* player);
	void	AddCmdGameCreateUnit(uint16_t id_model, double x, double y, uint8_t id_player);
	void	AddCmdGameDeleteUnit(uint32_t id_unit, uint8_t id_player);
	void	AddCmdGameOrderAttack(uint32_t u_target, uint8_t p_target,
				      uint32_t u_attacker, uint8_t p_attacker);
	int	AddCmdGameOrderMove(double x, double y, uint32_t id_unit, uint8_t id_player);
	int	AddCmdGameOrderFollow(uint32_t u_target, uint8_t p_target,
				      uint32_t u_attacker, uint8_t p_attacker);
	int	AddCmdGameOrderHold(uint32_t unit, uint8_t player);
	void	AddCmdGameSetGroup(uint8_t id_grp);
	void	AddCmdGameUseGroup(uint8_t id_grp, uint8_t id_ply);
	void	AddCmdGameSkill(double x, double y, uint32_t u_target, uint8_t p_target, uint16_t id_skill);
	void	AddCmdGameQueue(uint16_t id_unit, uint8_t id_player);
	void	AddCmdGameMessage(char* mess);
	int	AddCmdGameEndTick(void);

	int	AddCmdLobbyJoin(void);
	int	AddCmdLobbyJoinAccepted(uint8_t myID, Player* p);
	int	AddCmdLobbyJoinRefused(Socket*);
	int	AddCmdLobbyNewcomer(uint8_t ID, uint8_t faction, char* name);
	int	AddCmdLobbyUpdFaction(uint8_t ID, uint8_t faction);
	int	AddCmdLobbyUpdID(uint8_t ID, uint8_t newID);
	int	AddCmdLobbyMessage(char*);
	int	AddCmdLobbyLeave(uint8_t ID);
	int	AddCmdStartListen(void);
	int	AddCmdConnectEachOthers(void);
	int	AddCmdStartGame(void);
};

#endif
