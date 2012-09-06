#include "Set_Group.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Set_Group::Set_Group(std::vector<uint8_t>* d,
		     uint8_t idPlayer,
		     uint32_t t) : GameEvent(t),
				   __cmd(d),
				   __idPlayer(idPlayer)
{
}

Set_Group::~Set_Group()
{
	delete this->__cmd;
}

void	Set_Group::Apply()
{
	std::cout << "Set Group\n";
	Game*	g;
	std::vector<uint8_t>::iterator	it;
	std::vector<uint8_t>::iterator	end;

	g = Game::GetInstance();
	if (g->players[this->__idPlayer] != NULL)
	{
		it = this->__cmd->begin();
		end = this->__cmd->end();
		while (it != end)
		{
			g->players[this->__idPlayer]->SetGroup(*it);
			++it;
		}
	}
}

