#include "Create_Units.h"
#include "../Game.h"
#include "../Player.h"
#include "../../Database/Database.h"
#include "../Unit.h"
#include "../Building.h"
#include "../../Network/Order_Network.h"

Create_Units::Create_Units(std::vector<cmd_Create_Units>* d,
			   uint32_t t) : GameEvent(t),
					 __cmd(d)
{
}

Create_Units::~Create_Units()
{
	delete this->__cmd;
}

void	Create_Units::Apply()
{
	std::cout << "Create_Units\n";
	std::vector<cmd_Create_Units>::iterator	it;
	std::vector<cmd_Create_Units>::iterator	end;
	std::vector<dataPosition>::iterator	itP;
	std::vector<dataPosition>::iterator	endP;
	Database*	db;
	Game*		g;
	Unit*		u;
	Position	pos;
	Unit_Model*	model;

	g = Game::GetInstance();
	db = Database::GetInstance();
	it = this->__cmd->begin();
	end = this->__cmd->end();
	while (it != end)
	{
		model = db->getUnitModel((*it).id_model);
		itP = (*it).units.begin();
		endP = (*it).units.end();
		while (itP != endP)
		{
			if (model->GetType() < 3)
				u = new Unit(model, g->players[(*itP).id_player]);
			else
				u = new Building(model, g->players[(*itP).id_player]);
			pos.x = (*itP).x;
			pos.y = (*itP).y;
			std::cout << "Create_Unit("
				<< (*it).id_model << ", "
				<< (int)(*itP).id_player << ", "
				<< pos.x
				<< ";"
				<< pos.y
				<< ")"
				<< std::endl;
			u->SetPosition(pos);
			++itP;
		}
		++it;
	}
}

