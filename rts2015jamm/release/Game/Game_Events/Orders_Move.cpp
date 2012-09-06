#include "Orders_Move.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Orders_Move::Orders_Move(std::vector<cmd_Orders_Move>* d,
			 uint32_t t) : GameEvent(t),
				       __cmd(d)
{
}

Orders_Move::~Orders_Move()
{
	delete this->__cmd;
}

void	Orders_Move::Apply()
{
	std::cout << "Orders Move\n";
	Game*					g;
	std::vector<cmd_Orders_Move>::iterator	it;
	std::vector<cmd_Orders_Move>::iterator	end;
	std::vector<dataUnit>::iterator		itU;
	std::vector<dataUnit>::iterator		endU;

	g = Game::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		itU = (*it).units.begin();
		endU = (*it).units.end();
		while (itU != endU)
		{
			std::cout << "Order_Move("
				<< (*itU).id_unit
				<< "|"
				<< (*it).x
				<< ";"
				<< (*it).y
				<< ")"
				<< std::endl;
			if (g->players[(*itU).id_player] != NULL)
				g->players[(*itU).id_player]->SetUnitTargetMove((*itU).id_unit,
										(*it).x,
										(*it).y);
			++itU;
		}
		++it;
	}
}

