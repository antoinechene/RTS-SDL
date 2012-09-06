#include "Orders_Attack.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Orders_Attack::Orders_Attack(std::vector<cmd_Orders_Attack>* d,
			     uint32_t t) : GameEvent(t),
					   __cmd(d)
{
}

Orders_Attack::~Orders_Attack()
{
	delete this->__cmd;
}

void	Orders_Attack::Apply()
{
	std::cout << "Orders attack\n";
	Game*						g;
	std::vector<cmd_Orders_Attack>::iterator	it;
	std::vector<cmd_Orders_Attack>::iterator	end;
	std::vector<dataUnit>::iterator			itU;
	std::vector<dataUnit>::iterator			endU;
	Unit*						target;

	g = Game::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		itU = it->attackers.begin();
		endU = it->attackers.end();
		if (g->players[(*it).target.id_player] != NULL)
		{
			target = (*g->players[(*it).target.id_player]->GetUnits())[(*it).target.id_unit];
			while (itU != endU)
			{
				if (g->players[(*itU).id_player] != NULL)
					g->players[(*itU).id_player]->SetUnitTargetAttack((*itU).id_unit,
											  target);
				++itU;
			}
		}
		++it;
	}
}

