#ifndef __UNIT_MODEL_H__
#define __UNIT_MODEL_H__

#include <stdint.h>
#include <vector>

#ifndef	NULL
#define	NULL	0
#endif

enum	TYPE
{
	TYPE_NONE = 0,
	TYPE_GROUND,
	TYPE_AIR
};

enum	TARGET
{
	TARGET_NONE = 0,
	TARGET_GROUND,
	TARGET_AIR,
	TARGET_ALL
};

class	Sprite_Anime;
class	Script_Model;

class	Unit_Model
{
 public:
	Unit_Model(char** data);
	~Unit_Model()
	{
		delete[] this->_name;
	}

	Sprite_Anime*	GetSpriteAnime(void);
	void		SetID(uint16_t id);
	uint16_t	GetID();
	void		SetName(char* name);
	char*		GetName();
	void		SetFaction(uint8_t fact);
	uint8_t		GetFaction();
	void		SetType(uint8_t type);
	uint8_t		GetType();
	void		SetHPMax(uint32_t data);
	uint32_t	GetHPMax();
	void		SetPMMax(uint32_t data);
	uint32_t	GetPMMax();
	uint32_t	GetPopulation();
	void		SetAtk(uint32_t data);
	uint32_t	GetAtk();
	void		SetDef(uint32_t data);
	uint32_t	GetDef();
	void		SetViewRange(double data);
	double		GetViewRange();
	void		SetAspd(uint32_t data);
	uint32_t	GetAspd();
	void		SetArange(double data);
	double		GetArange();
	void		SetTarget(uint16_t data);
	uint16_t	GetTarget();
	void		SetSize(uint16_t data);
	uint16_t	GetSize();
	void		SetMoveSpeed(double data);
	double		GetMoveSpeed();
	void		SetGround(uint8_t data);
	uint8_t		GetGround();
	void		SetTargetType(uint8_t data);
	uint8_t		GetTargetType();
	void		SetAttackHandler(uint8_t data);
	uint8_t		GetAttackHandler();
	void		SetMoveHandler(uint8_t data);
	uint8_t		GetMoveHandler();
	uint8_t		GetStateMachineID();
	std::vector<Script_Model*>*	GetScripts(void);

 protected:
	Sprite_Anime*	_sprite;
	uint16_t	_ID;
	char*		_name;
	uint8_t		_faction;
	uint8_t		_type; // type sol/air
	uint8_t		_targetType;//??
	uint32_t	_HPMax;
	uint32_t	_PMMax;
	uint32_t	_population;
	uint32_t	_atk;
	int32_t		_def;
	uint32_t	_aspd;
	uint8_t		_Arange;
	uint8_t		_viewRange;
	uint16_t	_target;
	uint16_t	_size;
	double		_moveSpeed;
	uint8_t		_ground;
	uint8_t		_mhandler;
	uint8_t		_ahandler;
	uint8_t		_stateMachineID;
	std::vector<Script_Model*>	_scripts;
};

#endif
