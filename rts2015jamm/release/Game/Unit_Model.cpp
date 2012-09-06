#include "Unit_Model.h"
#include "Sprite_Anime.h"
#include "Scripts/Script_Manager.h"
#include "../Game/Data_Access.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

Unit_Model::Unit_Model(char** tab)
{
	int		i;
	int		max;
	Script_Manager*	sm;
	Script_Model*	script;

	sm = Script_Manager::GetInstance();
	this->_ID = atoi(tab[0]);
	this->_type = atoi(tab[1]);
	this->_faction = atoi(tab[2]);
	this->_name = strdup(tab[3]);
	this->_HPMax = atoi(tab[4]);
	this->_PMMax = atoi(tab[5]);
	this->_population = atoi(tab[17]);
	this->_atk = atoi(tab[6]);
	this->_def = atoi(tab[7]);
	this->_aspd = atoi(tab[8]);
	this->_Arange = (double)atoi(tab[9]) / 1000;
	this->_viewRange = (double)atoi(tab[10]) / 1000;
	this->_target = atoi(tab[11]);
	this->_moveSpeed = (double)atoi(tab[12]) / 1000;
	this->_size = atoi(tab[13]);
	this->_ground = atoi(tab[14]);
	this->_mhandler = atoi(tab[15]);
	this->_ahandler = atoi(tab[16]);
	this->_stateMachineID = atoi(tab[18]);
	max = atoi(tab[19]);
	i = 0;
	while (i < max)
	{
		script = sm->GetModel((SCRIPT_ID)atoi(tab[20 + i]));
		if (script == NULL)
			throw 1000;
		this->_scripts.push_back(script);
		++i;
	}

	this->_sprite = Data_Access::GetInstance()->GetUnitPicture(this->_ID);
	if (this->_sprite == NULL)
	{
		std::cout << "ERROR sprite\n";
	}
	std::cout << "Unit " << this->_name << " loaded in the DB" << std::endl;
}

Sprite_Anime*	Unit_Model::GetSpriteAnime(void)
{
	return this->_sprite;
}

void	Unit_Model::SetID(uint16_t id)
{
	this->_ID = id;
}

uint16_t	Unit_Model::GetID()
{
	return (this->_ID);
}

void	Unit_Model::SetName(char* name)
{
	delete[] this->_name;
	this->_name = strdup(name);
}

char*	Unit_Model::GetName()
{
	return (this->_name);
}

void	Unit_Model::SetFaction(uint8_t fact)
{
	this->_faction = fact;
}

uint8_t	Unit_Model::GetFaction()
{
	return (this->_faction);
}

void	Unit_Model::SetType(uint8_t type)
{
	this->_type = type;
}

uint8_t	Unit_Model::GetType()
{
	return (this->_type);
}

void	Unit_Model::SetHPMax(uint32_t hpmax)
{
	this->_HPMax = hpmax;
}

uint32_t	Unit_Model::GetHPMax()
{
	return (this->_HPMax);
}

void	Unit_Model::SetPMMax(uint32_t data)
{
	this->_PMMax = data;
}

uint32_t	Unit_Model::GetPMMax()
{
	return (this->_PMMax);
}

uint32_t	Unit_Model::GetPopulation()
{
	return this->_population;
}

void	Unit_Model::SetAtk(uint32_t data)
{
	this->_atk = data;
}

uint32_t	Unit_Model::GetAtk()
{
	return (this->_atk);
}

void	Unit_Model::SetDef(uint32_t data)
{
	this->_def = data;
}

uint32_t	Unit_Model::GetDef()
{
	return (this->_def);
}

void	Unit_Model::SetViewRange(double data)
{
	this->_viewRange = data;
}

double	Unit_Model::GetViewRange()
{
	return (this->_viewRange);
}

void	Unit_Model::SetAspd(uint32_t data)
{
	this->_aspd = data;
}

uint32_t	Unit_Model::GetAspd()
{
	return (this->_aspd);
}

void	Unit_Model::SetArange(double data)
{
	this->_Arange = data;
}

double	Unit_Model::GetArange()
{
	return (this->_Arange);
}

void	Unit_Model::SetTarget(uint16_t data)
{
	this->_target = data;
}

uint16_t	Unit_Model::GetTarget()
{
	return (this->_target);
}

void	Unit_Model::SetSize(uint16_t data)
{
	this->_size = data;
}

uint16_t	Unit_Model::GetSize()
{
	return (this->_size);
}

void	Unit_Model::SetMoveSpeed(double data)
{
	this->_moveSpeed = data;
}

double	Unit_Model::GetMoveSpeed()
{
	return this->_moveSpeed;
}

void	Unit_Model::SetGround(uint8_t data)
{
	this->_ground = data;
}

uint8_t	Unit_Model::GetGround()
{
	return this->_ground;
}

void	Unit_Model::SetTargetType(uint8_t data)
{
	this->_targetType = data;
}

uint8_t	Unit_Model::GetTargetType()
{
	return this->_targetType;
}

void	Unit_Model::SetAttackHandler(uint8_t data)
{
	this->_ahandler = data;
}

uint8_t	Unit_Model::GetAttackHandler()
{
	return this->_ahandler;
}

void	Unit_Model::SetMoveHandler(uint8_t data)
{
	this->_mhandler = data;
}

uint8_t	Unit_Model::GetMoveHandler()
{
	return this->_mhandler;
}

uint8_t	Unit_Model::GetStateMachineID()
{
	return this->_stateMachineID;
}

std::vector<Script_Model*>*	Unit_Model::GetScripts(void)
{
	return &(this->_scripts);
}

