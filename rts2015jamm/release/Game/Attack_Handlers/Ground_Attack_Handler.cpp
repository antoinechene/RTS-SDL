#include "Ground_Attack_Handler.h"
#include "../Game.h"
#include "../Unit.h"
#include "../Player.h"
#include "../Map.h"
#include "../Math_tools.h"
#include "../../Constantes.h"

Ground_Attack_Handler::Ground_Attack_Handler(Unit* u) : Attack_Handler(u), _lastAttack(0)
{
}

Ground_Attack_Handler::~Ground_Attack_Handler()
{
}

bool	Ground_Attack_Handler::CanAttack(Unit* u)
{
	double	xD;
	double	yD;
	double	xA;
	double	yA;
	double	dist;

	if (Game::GetInstance()->currentTick < this->_lastAttack + this->_unit->GetModel()->GetAspd())
		return false;
	if (u->GetPlayer() == this->_unit->GetPlayer())
//	if (u->GetPlayer() == this->_unit->GetPlayer() ||
//   u->GetPlayer()->GetTeam() == this->_unit->GetPlayer()->GetTeam())
		return false;
	if (this->_unit->GetModel()->GetTargetType() == TARGET_AIR)
		return false;
	u->GetPositionXY(&xD, &yD);
	this->_unit->GetPositionXY(&xA, &yA);
	dist = IDdistance(xD, yD, xA, yA) -
	       u->GetModel()->GetSize() -
	       this->_unit->GetModel()->GetSize() -
	       this->_unit->GetModel()->GetArange();
	return (dist <= 0);
}

int	Ground_Attack_Handler::Attack(Unit* u)
{
	if (!this->CanAttack(u))
		return 0;
	uint32_t	damage;

	damage = this->_unit->GetModel()->GetAtk() - u->GetModel()->GetDef();
	this->_lastAttack = Game::GetInstance()->currentTick;
	std::cout << "Unit(" << this->_unit->GetToken() <<  ") attacks Unit(" << u->GetToken() << ") "
		<< u->GetHP() << " - " << damage << std::endl;
	if (u->GetHP() <= damage)
	{
		std::cout << "Unit killed\n";
		u->Die();
		return 2;
	}
	else
		u->SetHP(u->GetHP() - damage);
	return 1;
}

