#include "Protocol_Manager.h"
#include "Sockets_manager.h"
#include "../Env.h"
#include "../Buffer.h"
#include "../Game/Game.h"
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
#include "../Screens/Lobby_Screen.h"
#include "../Screens/Game_Screen.h"
#include <iostream>
#include <unistd.h>

//ADD ABORT VAR EVERYWHERE
int	Protocol_Manager::__ReadCreateUnits(Buffer* b)
{
	/*static std::vector<cmd_Create_Units>::iterator	it;
	static std::vector<cmd_Create_Units>::iterator	end;
	static std::vector<cmd_Create_Units>*		str = NULL;
	static uint16_t					qty;
*/
	Read_Create_Units_Context*	ctxt;

	std::cout << "RCU" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Create_Units_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Create_Units_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<cmd_Create_Units>;
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		ctxt->it = ctxt->str->begin();
		ctxt->end = ctxt->str->end();
		std::cout << "RCU	qty=" << ctxt->qty << std::endl;
	}
	while (ctxt->it != ctxt->end)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < sizeof(uint16_t) + sizeof(uint16_t))
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(&(ctxt->qty), sizeof(uint16_t));
			b->getstr(&(*(ctxt->it)), sizeof(uint16_t));
			(*(ctxt->it)).units.resize(ctxt->qty);
			std::cout << "RCU	qty_unit" << ctxt->qty << std::endl;
			std::cout << "RCU	id_model" << (*(ctxt->it)).id_model << std::endl;
		}
		if (b->size() < sizeof(dataPosition) * ctxt->qty)
		{
			ctxt->step = 2;
			return -1;
		}
		std::cout << "bs=" << b->size() << std::endl;
		b->getstr(&(*(ctxt->it)).units.front(), sizeof(dataPosition) * ctxt->qty);
		std::cout << "RCU	#1("
			<< (*(ctxt->it)).units.front().x << ","
			<< (*(ctxt->it)).units.front().y << ")"
			<< (int)(*(ctxt->it)).units.front().id_player
			<< std::endl;
		++(ctxt->it);
		ctxt->step = 1;
	}
	Create_Units*	du;

	std::cout << "RCU	Tick=" << ctxt->tick + 1 << std::endl;
	du = new Create_Units(ctxt->str, ctxt->tick + 1);
	if (du == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(du);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadDeleteUnits(Buffer* b)
{
	Read_Delete_Units_Context*	ctxt;

	std::cout << "RDU" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Delete_Units_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Delete_Units_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<dataUnit>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RDU	qty=" << ctxt->qty << std::endl;
		ctxt->str->resize(ctxt->qty);
	}
	if (b->size() < sizeof(dataUnit) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	b->getstr(&(*(ctxt->str->begin())), sizeof(dataUnit) * ctxt->qty);
	Delete_Units*	du;

	du = new Delete_Units(ctxt->str, ctxt->tick + 1);
	if (du == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(du);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadChangeSelection(Buffer* b)
{
	Read_Change_Selection*	ctxt;
	std::cout << "RCS\n";

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Change_Selection;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Change_Selection*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
			return -1;
		ctxt->str = new std::vector<uint32_t>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		std::cout << "RCS	qty" << ctxt->qty << std::endl;
	}
	if (b->size() < sizeof(uint32_t) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	b->getstr(&(ctxt->str->front()), sizeof(uint32_t) * ctxt->qty);

	Change_Selection*	cs;

	cs = new Change_Selection(ctxt->str, this->workingSocket->player->GetID(), ctxt->tick + 1);
	if (cs == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(cs);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadOrdersAttack(Buffer* b)
{
/*	static std::vector<cmd_Orders_Attack>::iterator	it;
	static std::vector<cmd_Orders_Attack>::iterator	end;
	static std::vector<cmd_Orders_Attack>*		str = NULL;
	static uint16_t					qty;
*/
	Read_Orders_Attack_Context*	ctxt;
	std::cout << "ROA" << std::endl;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Attack_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Attack_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
			return -1;
		ctxt->str = new std::vector<cmd_Orders_Attack>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "ROA	qty=" << ctxt->qty << std::endl;
		ctxt->str->resize(ctxt->qty);
		ctxt->it = ctxt->str->begin();
		ctxt->end = ctxt->str->end();
	}
	while (ctxt->it != ctxt->end)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < sizeof(dataUnit) + sizeof(uint16_t))
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(&(*ctxt->it), sizeof(dataUnit));
			b->getstr(&(ctxt->qty), sizeof(uint16_t));
			(*ctxt->it).attackers.resize(ctxt->qty);
			std::cout << "ROA	TidU=" << (*ctxt->it).target.id_unit << std::endl;
			std::cout << "ROA	TidP=" << (int)(*ctxt->it).target.id_player << std::endl;
			std::cout << "ROA	Tqty=" << ctxt->qty << std::endl;
		}
		if (b->size() < sizeof(dataUnit) * ctxt->qty)
		{
			ctxt->step = 2;
			return -1;
		}
		b->getstr(&((*ctxt->it).attackers.front()), sizeof(dataUnit) * ctxt->qty);
		++ctxt->it;
		ctxt->step = 1;
	}
	Orders_Attack*	oa;

	oa = new Orders_Attack(ctxt->str, ctxt->tick + 1);
	if (oa == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(oa);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadOrdersMove(Buffer* b)
{
	Read_Orders_Move_Context*	ctxt;

	std::cout << "ROM" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Move_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Move_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<cmd_Orders_Move>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		ctxt->it = ctxt->str->begin();
		ctxt->end = ctxt->str->end();
		std::cout << "ROM	qty=" << ctxt->qty << std::endl;
	}
	while (ctxt->it != ctxt->end)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < sizeof(double) + sizeof(double) + sizeof(uint16_t))
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(&(*(ctxt->it)), sizeof(double) + sizeof(double));
			b->getstr(&(ctxt->qty), sizeof(uint16_t));
			(*ctxt->it).units.resize(ctxt->qty);
			std::cout << "ROM	X=" << (*ctxt->it).x << std::endl;
			std::cout << "ROM	Y=" << (*ctxt->it).y << std::endl;
			std::cout << "ROM	qty=" << ctxt->qty << std::endl;
		}
		if (b->size() < sizeof(dataUnit) * ctxt->qty)
		{
			ctxt->step = 2;
			return -1;
		}
		b->getstr(&((*ctxt->it).units[0]), sizeof(dataUnit) * ctxt->qty);
		++(ctxt->it);
		ctxt->step = 1;
	}
	Orders_Move*	om;

	om = new Orders_Move(ctxt->str, ctxt->tick + 1);
	if (om == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(om);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadOrdersFollow(Buffer* b)
{
	Read_Orders_Follow_Context*	ctxt;

	std::cout << "ROF" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Follow_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Follow_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<cmd_Orders_Follow>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		ctxt->it = ctxt->str->begin();
		ctxt->end = ctxt->str->end();
		std::cout << "ROF	qty=" << ctxt->qty << std::endl;
	}
	while (ctxt->it != ctxt->end)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < sizeof(dataUnit) + sizeof(uint16_t))
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(&(*(ctxt->it)), sizeof(dataUnit));
			b->getstr(&(ctxt->qty), sizeof(uint16_t));
			(*ctxt->it).units.resize(ctxt->qty);
			std::cout << "ROF	u=" << (*ctxt->it).target.id_unit << std::endl;
			std::cout << "ROF	p=" << (int)(*ctxt->it).target.id_player << std::endl;
			std::cout << "ROF	qty=" << ctxt->qty << std::endl;
		}
		if (b->size() < sizeof(dataUnit) * ctxt->qty)
		{
			ctxt->step = 2;
			return -1;
		}
		b->getstr(&((*ctxt->it).units[0]), sizeof(dataUnit) * ctxt->qty);
		++(ctxt->it);
		ctxt->step = 1;
	}
	Orders_Follow*	om;

	om = new Orders_Follow(ctxt->str, ctxt->tick + 1);
	if (om == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(om);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadOrdersHold(Buffer* b)
{
	Read_Orders_Hold_Context*	ctxt;

	std::cout << "ROH" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Hold_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Hold_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<dataUnit>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		ctxt->it = ctxt->str->begin();
		ctxt->end = ctxt->str->end();
		std::cout << "ROH	qty=" << ctxt->qty << std::endl;
	}
	while (ctxt->it != ctxt->end)
	{
		if (b->size() < sizeof(dataUnit) * ctxt->qty)
		{
			ctxt->step = 1;
			return -1;
		}
		b->getstr(&(*ctxt->it), sizeof(dataUnit) * ctxt->qty);
		++(ctxt->it);
		ctxt->step = 1;
	}
	Orders_Hold*	oh;

	oh = new Orders_Hold(ctxt->str, ctxt->tick + 1);
	if (oh == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(oh);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadSetGroup(Buffer* b)
{
	Read_Set_Group_Context*	ctxt;

	std::cout << "RSG" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Set_Group_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Set_Group_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t) + sizeof(uint32_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new std::vector<uint8_t>();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->tick), sizeof(uint32_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		ctxt->str->resize(ctxt->qty);
		std::cout << "RSG	qty=" << ctxt->qty << std::endl;
	}
	if (b->size() < sizeof(uint8_t) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	b->getstr(&(ctxt->str->front()), sizeof(uint8_t) * ctxt->qty);
	Set_Group*	du;

	du = new Set_Group(ctxt->str, this->workingSocket->player->GetID(), ctxt->tick + 1);
	if (du == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(du);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadUseGroup(Buffer* b)
{
/*	uint8_t	idGroup;
	uint8_t	idPlayer;
*/
	Read_Use_Group_Context*	ctxt;

	std::cout << "RUG" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Use_Group_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Use_Group_Context*)this->workingSocket->readContext;
	if (b->size() < sizeof(uint8_t) + sizeof(uint32_t))
		return -1;
	b->getstr(&(ctxt->tick), sizeof(uint32_t));
	b->getstr(&(ctxt->idGroup), sizeof(uint8_t));
	std::cout << "RUG	idG" << (int)ctxt->idGroup << std::endl;
	Use_Group*	du;

	du = new Use_Group(ctxt->idGroup,
			   this->workingSocket->player->GetID(),
			   ctxt->tick + 1);
	if (du == NULL)
		return -1;
	Game::GetInstance()->gameEvents.push_back(du);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadOrderSkill(Buffer* b)
{
/*	static std::vector<uint16_t>::iterator	it;
	static std::vector<uint16_t>::iterator	end;
	static cmd_Skill*			str = NULL;
*/
	(void)b;
/*	Read_Orders_Skill_Context*	ctxt;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Skill_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Skill_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(double) + sizeof(double) + sizeof(dataUnit) + sizeof(uint16_t))
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new cmd_Skill();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&str, sizeof(double) + sizeof(double) + sizeof(dataUnit) + sizeof(uint16_t));
		ctxt->str->id_skill.resize(ctxt->str->qty);
		ctxt->it = ctxt->str->id_skill.begin();
		ctxt->end = ctxt->str->id_skill.end();
	}
	while (ctxt->it != ctxt->end)
	{
		if (b->size() < sizeof(uint16_t) * ctxt->str->qty)
		{
			ctxt->step = 1;
			return -1;
		}
		b->getstr(&(*(ctxt->it)), sizeof(uint16_t) * ctxt->str->qty);
		++(ctxt->it);
		ctxt->step = 1;
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	*/
	return 0;
}

int	Protocol_Manager::__ReadOrderQueueUnit(Buffer* b)
{
//	static cmd_Queue_unit*	ctxt->str = NULL;
/*	Read_Orders_Queue_Units_Context*	ctxt;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Orders_Queue_Units_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Orders_Queue_Units_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < 2)
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new cmd_Queue_unit;
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->str->qty), 2);
		ctxt->str->buildings.resize(ctxt->str->qty);
	}
	if (b->size() < sizeof(dataUnit) * ctxt->str->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	b->getstr(&(ctxt->str->buildings), sizeof(dataUnit) * ctxt->str->qty);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	*/
	(void)b;
	return 0;
}

/*int	__ReadGameMessage(Buffer* b)
{
	static std::vector<char*>::iterator	it;
	static std::vector<char*>::iterator	end;
	static cmd_Message*			str = NULL;

	if (ctxt->step == 0)
	{
		if (b->size() < 2)
		{
			ctxt->step = 0;
			return -1;
		}
		ctxt->str = new cmd_Message();
		if (ctxt->str == NULL)
			return -1;
		b->getstr(&(ctxt->str.qty), 2);
		str.mess.resize(ctxt->str.qty);
		it = str.mess.begin();
		ctxt->end = str.mess.end();
	}
	while (ctxt->it != ctxt->end)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < )
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(
		}
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}*/

int	Protocol_Manager::__ReadGameEndTick(void)
{
	std::cout << "RGET\n";
	Game*	gm;

	gm = Game::GetInstance();
	if (!gm->IsGameRunning())
		return 0;
	this->workingSocket->player->SetReadyTick(true);
	return 0;
}


int	Protocol_Manager::__ReadLobbyJoin(Buffer* b)
{
	int	i;
	Game*	gm;
	char	nick[20];

	std::cout << "RLJ" << std::endl;
	if (b->size() < sizeof(char) * 20)
	{
		return -1;
	}
	gm = Game::GetInstance();
	b->getstr(nick, sizeof(char) * 20);
	if (nick[0] != 0)
	{
		i = gm->AddPlayer(nick);
		if (i != -1)
		{
			gm->players[i]->SetSocket(this->workingSocket);
			this->prot.AddCmdLobbyJoinAccepted(i, gm->players[i]);
			this->prot.AddCmdLobbyNewcomer(i, 0, gm->players[i]->GetName());
			return 0;
		}
	}
	this->prot.AddCmdLobbyJoinRefused(this->workingSocket);
	return 0;
}

int	Protocol_Manager::__ReadLobbyJoinAccepted(Buffer* b)
{
/*	static uint32_t	hash;
	static uint16_t	qty;
	static uint8_t	ID;
	static uint8_t	faction;
	static char	name[20];
	static char*	mapName;
*/
	Read_Lobby_Join_Accepted_Context*	ctxt;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Join_Accepted_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Join_Accepted_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t))
			return -1;
		b->getstr(&(ctxt->hash), sizeof(uint32_t));
		b->getstr(&(ctxt->ID), sizeof(uint8_t));
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLJA	hash=" << ctxt->hash << std::endl;
		std::cout << "RLJA	ID=" << (int)ctxt->ID << std::endl;
		std::cout << "RLJA	length=" << ctxt->qty << std::endl;
	}
	Game*	gm;
	bool	avoid;

	gm = Game::GetInstance();
	avoid = false;
	if (gm->IsInLobby()) // Game already created
		avoid = true;
	if (ctxt->step <= 1)
	{
		if (b->size() < sizeof(char) * ctxt->qty)
		{
			ctxt->step = 1;
			return -1;
		}
		ctxt->mapName = new char[qty + 1];
		if (ctxt->mapName == NULL)
		{
			ctxt->step = 1;
			return -1;
		}
		b->getstr(ctxt->mapName, ctxt->qty);
		ctxt->mapName[ctxt->qty] = 0;
		std::cout << "RLJA	map=" << ctxt->mapName << std::endl;
		if (avoid == false &&
		    gm->InitLobby(ctxt->mapName, ctxt->ID) == -1)
		{
			this->prot.AddCmdLobbyLeave(ctxt->ID);
//			delete ctxt;
//			this->workingSocket->readContext = NULL;
//			return 0;
			avoid = true;
		}
		gm->myID = ctxt->ID;
	}
	if (ctxt->step <= 2)
	{
		if (b->size() < sizeof(uint8_t))
		{
			ctxt->step = 2;
			return -1;
		}
		ctxt->qty = 0;
		b->getstr(&(ctxt->qty), sizeof(uint8_t));
	}
	if (b->size() < (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(char) * 20) * ctxt->qty)
	{
		ctxt->step = 3;
		return -1;
	}
	while (ctxt->qty > 0)
	{
		b->getstr(&(ctxt->ID), sizeof(uint8_t));
		b->getstr(&(ctxt->faction), sizeof(uint8_t));
		b->getstr(ctxt->name, sizeof(char) * 20);
		if (avoid == false)
			gm->AddPlayer(ctxt->name, ctxt->ID);
		std::cout << "	ID" << (int)ctxt->ID << std::endl;
		std::cout << "	fac" << (int)ctxt->faction << std::endl;
		std::cout << "	nam" << ctxt->name << std::endl;
		--(ctxt->qty);
	}
	if (avoid == false)
	{
		gm->players[0]->SetSocket(this->workingSocket);
		this->workingSocket->player = gm->players[0]; //#TODO Gerer le cas ou le host est different de 0 (Quand on rejoind le serveur alors que le host a bouge)
		gm->AmIHost = false;
		Env*	e;
		e = Env::GetInstance();
		e->SwitchScreen(e->lobbyScreen);
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadLobbyJoinRefused(void)
{
	//#TODO CLOSE GAME, CLOSE CONNECTIONS, CLEAN E V E R Y THING!
	return 0;
}

int	Protocol_Manager::__ReadLobbyNewcomer(Buffer* b)
{
/*	static uint16_t				qty;
	static uint8_t				ID;
	static uint8_t				faction;
	static char				name[20];
*/
	Read_Lobby_Newcomer_Context*	ctxt;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Newcomer_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Newcomer_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t))
			return -1;
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLN	qty=" << ctxt->qty << std::endl;
	}
	if (b->size() < (sizeof(uint8_t) + sizeof(uint8_t) + sizeof(char) * 20) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	while (ctxt->qty > 0)
	{
		b->getstr(&(ctxt->ID), sizeof(uint8_t));
		b->getstr(&(ctxt->faction), sizeof(uint8_t));
		b->getstr(&(ctxt->name), sizeof(char) * 20);
		std::cout << "	ID" << (int)ctxt->ID << std::endl;
		std::cout << "	fac" << (int)ctxt->faction << std::endl;
		std::cout << "	nam" << ctxt->name << std::endl;
		--(ctxt->qty);
	}
	if (Game::GetInstance()->IsInLobby())
		Game::GetInstance()->AddPlayer(ctxt->name, ctxt->ID);
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadLobbyUpdFaction(Buffer* b)
{
/*	static uint16_t	qty;
	static uint8_t	ID;
	static uint8_t	faction;
*/
	Read_Lobby_Upd_Faction_Context*	ctxt;
	Game*				gm;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Upd_Faction_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Upd_Faction_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t))
			return -1;
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLUF	qty=" << ctxt->qty << std::endl;
	}
	if (b->size() < (sizeof(uint8_t) + sizeof(uint8_t)) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	gm = Game::GetInstance();
	while (ctxt->qty > 0)
	{
		b->getstr(&(ctxt->ID), sizeof(uint8_t));
		b->getstr(&(ctxt->faction), sizeof(uint8_t));
		std::cout << "	ID" << (int)ctxt->ID << std::endl;
		std::cout << "	fac" << (int)ctxt->faction << std::endl;
		if (ctxt->ID < gm->countPlayers)
			gm->UpdatePlayerFaction(ctxt->ID, ctxt->faction);
		--(ctxt->qty);
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadLobbyUpdID(Buffer* b)
{
/*	static uint16_t	qty;
	static uint8_t	ID;
	static uint8_t	newID;
*/
	Read_Lobby_Upd_ID_Context*	ctxt;
	Game*				gm;
	Env*				e;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Upd_ID_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Upd_ID_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t))
			return -1;
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLUI	qty=" << ctxt->qty << std::endl;
	}
	if (b->size() < (sizeof(uint8_t) + sizeof(uint8_t)) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	gm = Game::GetInstance();
	e = Env::GetInstance();
	while (ctxt->qty > 0)
	{
		b->getstr(&(ctxt->ID), sizeof(uint8_t));
		b->getstr(&(ctxt->newID), sizeof(uint8_t));
		std::cout << "	ID" << (int)ctxt->ID << std::endl;
		std::cout << "	newID" << (int)ctxt->newID << std::endl;
		if (ctxt->ID < gm->countPlayers)
		{
			if (gm->AmIHost == true)
				this->prot.AddCmdLobbyUpdID(ctxt->ID, ctxt->newID);
			else
				gm->UpdatePlayerID(ctxt->ID, ctxt->newID);
		}
		--(ctxt->qty);
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadLobbyMessage(Buffer* b)
{
/*	static uint16_t	qty;
	static uint16_t	length;
	static char*	m;
*/
	Read_Lobby_Message_Context*	ctxt;

	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Message_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Message_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t))
			return -1;
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLM	qty=" << ctxt->qty << std::endl;
	}
	while (ctxt->qty > 0)
	{
		if (ctxt->step <= 1)
		{
			if (b->size() < sizeof(uint16_t))
			{
				ctxt->step = 1;
				return -1;
			}
			b->getstr(&(ctxt->length), sizeof(uint16_t));
			std::cout << "RLM	l=" << ctxt->length << std::endl;
		}
		if (b->size() < sizeof(char) * ctxt->length)
		{
			ctxt->step = 2;
			return -1;
		}
		ctxt->m = new char[ctxt->length + 1];
		if (ctxt->m == NULL)
		{
			ctxt->step = 2;
			return -1;
		}
		b->getstr(ctxt->m, ctxt->length);
		std::cout << "	m" << ctxt->m << std::endl;
		delete ctxt->m;
		--(ctxt->qty);
		ctxt->step = 1;
	}
	delete this->workingSocket->readContext;
	this->workingSocket->readContext = NULL;
	return 0;
}

int	Protocol_Manager::__ReadLobbyLeave(Buffer* b)
{
	Read_Lobby_Leave_Context*	ctxt;

	std::cout << "RLL" << std::endl;
	if (this->workingSocket->readContext == NULL)
	{
		this->workingSocket->readContext = new Read_Lobby_Leave_Context;
		if (this->workingSocket->readContext == NULL)
			return -1;
	}
	ctxt = (Read_Lobby_Leave_Context*)this->workingSocket->readContext;
	if (ctxt->step == 0)
	{
		if (b->size() < sizeof(uint16_t))
			return -1;
		b->getstr(&(ctxt->qty), sizeof(uint16_t));
		std::cout << "RLL	qty=" << ctxt->qty << std::endl;
	}
	if (b->size() < sizeof(uint8_t) * ctxt->qty)
	{
		ctxt->step = 1;
		return -1;
	}
	Sockets_manager*	sm;
	Game*			gm;
	uint8_t			ID;

	gm = Game::GetInstance();
	sm = Sockets_manager::GetInstance();
	while (ctxt->qty > 0)
	{
		b->getstr(&ID, sizeof(uint8_t));
		std::cout << "	ID" << (int)ID << std::endl;
		if (gm->IsInLobby())
		{
			if (gm->players[ID] != NULL)
			{
				std::cout << "	deleting..\n";
//				sm->DeleteSocket(this->workingSocket);
				gm->DelPlayer(ID);
//				sm->DeleteSocket(gm->players[ID]->GetSocket());
			}
			if (gm->AmIHost == true)
				this->prot.AddCmdLobbyLeave(ID);
		}
		--(ctxt->qty);
	}
	return 0;
}

int	Protocol_Manager::__ReadStartListen(void)
{
	Sockets_manager*	sm;
	Game*	gm;
	Socket*	skt;
	uint8_t	i;
	uint8_t	countHostingPlayers;
	uint8_t	countPlayers;

	std::cout << "RSL" << std::endl;
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	if (gm->AmIHost == false)
	{
		sm = Sockets_manager::GetInstance();
		skt = sm->ListenSocket(PORT);
		if (skt == NULL)
		{
			std::cerr << "Error listening port " << PORT << std::endl;
			this->prot.AddCmdLobbyLeave(gm->myID);
		}
		else
			this->prot.AddCmdStartListen();
	}
	else
	{
		i = 0;
		countPlayers = 0;
		countHostingPlayers = 0;
		while (i < gm->countPlayers)
		{
			if (gm->players[i] != NULL)
			{
				if (gm->players[i]->GetHosting() == true)
					++countHostingPlayers;
				else if (gm->players[i]->GetSocket() == this->workingSocket)
				{
					gm->players[i]->SetHosting(true);
					++countHostingPlayers;
				}
				++countPlayers;
			}
			++i;
		}
		std::cout << "	CP" << (int)countPlayers << std::endl;
		std::cout << "	CHP" << (int)countHostingPlayers << std::endl;
		if (countPlayers == countHostingPlayers)
			this->prot.AddCmdConnectEachOthers();
	}
	return 0;
}

int	Protocol_Manager::__ReadConnectEachOthers(Buffer* b)
{
	Sockets_manager*	sm;
	Game*	gm;
	Socket*	skt;
	uint8_t	i;
	char	ip[16];
	int	countPlayers;
	int	countConnectedPlayers;
	bool	abort;

	std::cout << "RCEO" << std::endl;
	sm = Sockets_manager::GetInstance();
	gm = Game::GetInstance();
	abort = false;
	if (!gm->IsInLobby())
		abort = true;
	if (gm->AmIHost == true)
	{
		i = 0;
		countPlayers = 0;
		countConnectedPlayers = 0;
		while (i < gm->countPlayers)
		{
			if (gm->players[i] != NULL)
			{
				if (gm->players[i]->GetConnectedToAll() == true)
					++countConnectedPlayers;
				else if (gm->players[i]->GetSocket() == this->workingSocket)
				{
					gm->players[i]->SetConnectedToAll(true);
					++countConnectedPlayers;
				}
				++countPlayers;
			}
			++i;
		}
		std::cout << "CCP	" << countConnectedPlayers << std::endl;
		std::cout << "CP	" << countPlayers << std::endl;
		if (countConnectedPlayers == countPlayers)
			this->prot.AddCmdStartGame();
		return 0;
	}

	i = 0;
	countPlayers = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL)
			++countPlayers;
		++i;
	}
	--countPlayers; // Except host
	if (b->size() < (sizeof(char) * 16 + sizeof(uint8_t)) * countPlayers)
		return -1;
	while (countPlayers > 0)
	{
		b->getstr(&i, sizeof(uint8_t));
		b->getstr(ip, sizeof(char) * 16);
		std::cout << "	ID" << (int)i << std::endl;
		std::cout << "	IP" << ip << std::endl;
		if (gm->players[i] != NULL && abort == false && i != gm->myID)
		{
			skt = sm->ConnectSocket(PORT, ip);
			if (skt == NULL)
				std::cerr << "Error connecting player[" << (int)i << "] with " << ip << std::endl;
			else
				gm->players[i]->SetSocket(skt);
			gm->players[i]->SetIP(ip);
		}
		--countPlayers;
	}
	this->prot.AddCmdConnectEachOthers();
	return 0;
}

int	Protocol_Manager::__ReadStartGame(Buffer* b)
{
	Game*	gm;
	uint8_t	i;
	uint8_t	IP;

	std::cout << "RSG" << std::endl;
	gm = Game::GetInstance();
	if (!gm->IsInLobby())
		return 0;
	IP = 0;
	i = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL)
			++IP;
		++i;
	}
	if (b->size() < sizeof(uint8_t) * IP)
		return -1;
	i = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL)
		{
			b->getstr(&IP, sizeof(uint8_t));
			gm->players[i]->SetInitialPos(IP);
			std::cout << "Player[" << (int)i << "] " << (int)IP << std::endl;
		}
		++i;
	}
	gm->StartGame();
	return 0;
}

