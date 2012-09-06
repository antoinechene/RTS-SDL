#include "Use_Group.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

Use_Group::Use_Group(uint8_t idGroup,
		     uint8_t idPlayer,
		     uint32_t t) : GameEvent(t),
				   __idGroup(idGroup),
				   __idPlayer(idPlayer)
{
}

Use_Group::~Use_Group()
{
}

void	Use_Group::Apply()
{
	std::cout << "Use Group\n";
	Game*	g;

	g = Game::GetInstance();
	if (g->players[this->__idPlayer] != NULL)
		g->players[this->__idPlayer]->UseGroup(this->__idGroup);
}

