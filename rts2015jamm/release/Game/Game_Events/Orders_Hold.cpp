#include "Orders_Hold.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Orders_Hold::Orders_Hold(std::vector<dataUnit>* d,
			 uint32_t t) : GameEvent(t),
				       __cmd(d)
{
}

Orders_Hold::~Orders_Hold()
{
	delete this->__cmd;
}

void	Orders_Hold::Apply()
{
	std::cout << "Orders Hold\n";
	Game*				g;
	std::vector<dataUnit>::iterator	it;
	std::vector<dataUnit>::iterator	end;

	g = Game::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		std::cout << "Order_Hold("
			<< (*it).id_unit
			<< ";"
			<< (*it).id_player
			<< ")"
			<< std::endl;
		if (g->players[(*it).id_player] != NULL)
			g->players[(*it).id_player]->SetUnitHold((*it).id_unit);
		++it;
	}
}

