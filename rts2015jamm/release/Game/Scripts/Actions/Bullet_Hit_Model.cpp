#include "Bullet_Hit_Model.h"
#include "../Script.hpp"

Bullet_Hit_Model::Bullet_Hit_Model() : _HPdamage(0)
{
/*	this->SetRelX(0);
	this->SetRelY(0);
	this->SetRadius(0);
	this->SetDuration(0);
	this->SetRedundant(RDDT_FALSE);
	this->SetCancelable(CCBL_TRUE);
	this->SetPersistance(PRST_FALSE);*/
}

Bullet_Hit_Model::~Bullet_Hit_Model()
{
}

Action*	Bullet_Hit_Model::Clone(void)
{
	return 0;
}

int	Bullet_Hit_Model::_Apply(Script* s)
{
	(void)s;
//	s->GetTarget()->SetHP(GetHP - HP);
	return 0;
}
