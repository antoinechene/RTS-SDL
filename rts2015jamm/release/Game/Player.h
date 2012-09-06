#ifndef	__PLAYER_H__
#define __PLAYER_H__

#include "../Constantes.h"
#include "Group.h"
#include "Map.h"
#include <vector>
#include <list>
#include <queue>
#include <stdint.h>


class	Resource_Model;
class	Unit;
class	Socket;

struct	Resource
{
	Resource_Model*	model;
	uint32_t	qty;
};

class	Player
{
 public:
	Player(uint8_t ID, char* name, uint8_t team, uint8_t Faction);
	~Player();

	void			SetID(uint8_t);
	void			SetName(char*);
	void			SetTeam(uint8_t);
	void			SetToken(uint32_t);
	void			SetFaction(uint8_t);
	void			SetSocket(Socket*);
	void			SetIP(char*);
	void			SetInitialPos(uint8_t);
	void			SetHosting(bool);
	void			SetConnectedToAll(bool);
	void			SetReadyTick(bool);
	void			SetUnitTargetMove(uint32_t ID, double x, double y);
	void			SetUnitTargetFollow(uint32_t ID, Unit*);
	void			SetUnitHold(uint32_t ID);
	void			SetUnitTargetAttack(uint32_t ID, double x, double y);
	void			SetUnitTargetAttack(uint32_t ID, Unit*);

	uint8_t			GetID(void);
	uint8_t			GetTeam(void);
	uint32_t		GetToken(void);
	uint8_t			GetFaction(void);
	char*			GetName(void);
	Socket*			GetSocket(void);
	char*			GetIP(void);
	uint8_t			GetInitialPos(void);
	bool			GetHosting(void);
	bool			GetConnectedToAll(void);
	bool			GetReadyTick(void);
	uint32_t		GetPopulation(void);

	void			SetGroup(int idGroup);
	void			UseGroup(int idGroup);
	Group*			GetGroup(int idGroup);

	void			AddUnit2Selection(Unit*, uint32_t last);
	void			DelUnitSelection(Unit*);
	std::list<Unit*>*	GetSelection(void);
	void			ClearSelection(void);

	uint32_t		AddUnit(Unit*);
	std::vector<Unit*>*	GetUnits(void);
	void			DelUnit(uint32_t token);

	Resource		GetResource(Resource_Model*);
	void			SetResource(Resource_Model*, uint32_t qty);
	void			AddResource(Resource_Model*, uint32_t delta);

 protected:
	void	_AddPopulation(uint32_t delta);

 protected:
	uint8_t			_ID;
	uint8_t			_team;
	uint32_t		_tokenGiver;
	uint8_t			_faction;
	char			_name[21];
	char			_ip[16];
	uint32_t		_population;
	std::vector<Unit*>	_units;
	std::queue<uint32_t>	_tokensLeft;
	Group			_groups[MAX_GROUPS];
	std::list<Unit*>	_selection;
	std::vector<Resource>	_resources;
	Socket*			_socket;
	uint8_t			_initialPos;
	bool			_readyTick;

	// Lobby variables
	bool			_hosting;
	bool			_connectedToAll;
};

#endif
