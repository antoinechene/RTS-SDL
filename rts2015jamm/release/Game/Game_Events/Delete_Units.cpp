#include "Delete_Units.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"
#include "../Unit.h"

Delete_Units::Delete_Units(std::vector<dataUnit>* d,
			   uint32_t t) : GameEvent(t),
					 __cmd(d)
{
}

Delete_Units::~Delete_Units()
{
	delete this->__cmd;
}

void	Delete_Units::Apply()
{
	std::vector<dataUnit>::iterator	it;
	std::vector<dataUnit>::iterator	end;
	std::vector<Unit*>*		units;
	Game*	g;

	g = Game::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		if (g->players[(*it).id_player] != NULL)
		{
			units = g->players[(*it).id_player]->GetUnits();
			(*units)[(*it).id_unit]->Die();
		}
		++it;
	}
}

