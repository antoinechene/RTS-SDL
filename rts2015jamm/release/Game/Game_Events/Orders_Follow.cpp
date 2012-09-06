#include "Orders_Follow.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Orders_Follow::Orders_Follow(std::vector<cmd_Orders_Follow>* d,
			 uint32_t t) : GameEvent(t),
				       __cmd(d)
{
}

Orders_Follow::~Orders_Follow()
{
	delete this->__cmd;
}

void	Orders_Follow::Apply()
{
	std::cout << "Orders Follow\n";
	Game*						g;
	std::vector<cmd_Orders_Follow>::iterator	it;
	std::vector<cmd_Orders_Follow>::iterator	end;
	std::vector<dataUnit>::iterator			itU;
	std::vector<dataUnit>::iterator			endU;
	Unit*						target;

	g = Game::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		itU = (*it).units.begin();
		endU = (*it).units.end();
		target = (*g->players[(*it).target.id_player]->GetUnits())[(*it).target.id_unit];
		while (itU != endU)
		{
			std::cout << "Order_Follow("
				<< (*itU).id_unit
				<< " -> "
				<< (*it).target.id_unit
				<< ";"
				<< (int)(*it).target.id_player
				<< ")"
				<< std::endl;
			if (g->players[(*itU).id_player] != NULL)
				g->players[(*itU).id_player]->SetUnitTargetFollow((*itU).id_unit,
										  target);
			++itU;
		}
		++it;
	}
}

