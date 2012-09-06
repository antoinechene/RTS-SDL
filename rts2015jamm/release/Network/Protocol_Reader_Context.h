#ifndef	__PROTOCOL_READER_CONTEXT_H__
#define	__PROTOCOL_READER_CONTEXT_H__

#include <vector>
#include <stdint.h>

class	cmd_Create_Units;
class	dataUnit;
class	cmd_Orders_Attack;
class	cmd_Orders_Move;
class	cmd_Orders_Follow;
class	cmd_Set_Group;

class	Protocol_Reader_Context
{
	public:
		Protocol_Reader_Context();
		~Protocol_Reader_Context();
	public:
		uint32_t	step;
};

class	Read_Change_Selection : public Protocol_Reader_Context
{
	public:
		std::vector<uint32_t>*	str;
		uint32_t		tick;
		uint16_t		qty;
};

class	Read_Create_Units_Context : public Protocol_Reader_Context
{
	public:
		std::vector<cmd_Create_Units>::iterator	it;
		std::vector<cmd_Create_Units>::iterator	end;
		std::vector<cmd_Create_Units>*		str;
		uint32_t				tick;
		uint16_t				qty;
};

class	Read_Delete_Units_Context : public Protocol_Reader_Context
{
	public:
		std::vector<dataUnit>*	str;
		uint32_t		tick;
		uint16_t		qty;
};

class	Read_Orders_Attack_Context : public Protocol_Reader_Context
{
	public:
		std::vector<cmd_Orders_Attack>::iterator	it;
		std::vector<cmd_Orders_Attack>::iterator	end;
		std::vector<cmd_Orders_Attack>*			str;
		uint32_t					tick;
		uint16_t					qty;
};

class	Read_Orders_Move_Context : public Protocol_Reader_Context
{
	public:
		std::vector<cmd_Orders_Move>::iterator	it;
		std::vector<cmd_Orders_Move>::iterator	end;
		std::vector<cmd_Orders_Move>*		str;
		uint32_t				tick;
		uint16_t				qty;
};

class	Read_Orders_Follow_Context : public Protocol_Reader_Context
{
	public:
		std::vector<cmd_Orders_Follow>::iterator	it;
		std::vector<cmd_Orders_Follow>::iterator	end;
		std::vector<cmd_Orders_Follow>*			str;
		uint32_t					tick;
		uint16_t					qty;
};

class	Read_Orders_Hold_Context : public Protocol_Reader_Context
{
	public:
		std::vector<dataUnit>::iterator	it;
		std::vector<dataUnit>::iterator	end;
		std::vector<dataUnit>*		str;
		uint32_t			tick;
		uint16_t			qty;
};

class	Read_Set_Group_Context : public Protocol_Reader_Context
{
	public:
		std::vector<uint8_t>*	str;
		uint32_t		tick;
		uint16_t		qty;
};

class	Read_Use_Group_Context : public Protocol_Reader_Context
{
	public:
		uint32_t	tick;
		uint8_t		idGroup;
};

class	Read_Orders_Skill_Context: public Protocol_Reader_Context
{
	public:
//		std::vector<cmd_Orders_Move>::iterator	it;
//		std::vector<cmd_Orders_Move>::iterator	end;
//		std::vector<cmd_Orders_Move>*		str;
};

class	Read_Lobby_Join_Accepted_Context : public Protocol_Reader_Context
{
	public:
		uint32_t	hash;
		uint16_t	qty;
		uint8_t		ID;
		uint8_t		faction;
		char		name[20];
		char*		mapName;
};

class	Read_Lobby_Newcomer_Context : public Protocol_Reader_Context
{
	public:
		uint16_t	qty;
		uint8_t		ID;
		uint8_t		faction;
		char		name[20];
};

class	Read_Lobby_Upd_Faction_Context : public Protocol_Reader_Context
{
	public:
		uint16_t	qty;
		uint8_t		ID;
		uint8_t		faction;
};

class	Read_Lobby_Upd_ID_Context : public Protocol_Reader_Context
{
	public:
		uint16_t	qty;
		uint8_t		ID;
		uint8_t		newID;
};

class	Read_Lobby_Message_Context : public Protocol_Reader_Context
{
	public:
		uint16_t	qty;
		uint16_t	length;
		char*		m;
};

class	Read_Lobby_Leave_Context : public Protocol_Reader_Context
{
	public:
		std::vector<uint8_t>	str;
		uint16_t		qty;
};
/*
class	Read_Connect_Each_Others_Context : public Protocol_Reader_Context
{
	public:
		uint16_t		qty;
};*/

#endif
