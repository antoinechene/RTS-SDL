#include "Change_Selection.h"
#include "../../Network/Order_Network.h"
#include "../Game.h"
#include "../Player.h"

class	Unit;

Change_Selection::Change_Selection(std::vector<uint32_t>* cmd,
				   uint8_t player,
				   uint32_t t) : GameEvent(t),
						 __cmd(cmd),
						 __player(player)
{
}

Change_Selection::~Change_Selection()
{
	delete this->__cmd;
}

void	Change_Selection::Apply()
{
	std::cout << "Change Selection\n";
	std::vector<uint32_t>::iterator	it;
	std::vector<uint32_t>::iterator	end;
	Game*			gm;
	Player*			p;
	std::list<Unit*>*	selection;
	std::vector<Unit*>*	units;

	gm = Game::GetInstance();
	p = gm->players[this->__player];
	if (p != NULL)
	{
		p->ClearSelection();
		selection = p->GetSelection();
		units = p->GetUnits();
		it = this->__cmd->begin();
		end = this->__cmd->end();
		while (it != end)
		{
			p->AddUnit2Selection((*units)[*it], 0);
			++it;
		}
	}
}

