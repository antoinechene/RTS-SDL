#include "Building.h"
#include "../Database/Database.h"
#include "Game.h"
#include "../Constantes.h"
#include "../Network/Protocol_Manager.h"
#include "Player.h"
#include "../Env.h"
#include "../Screens/Game_Screen.h"

Building::Building(Unit_Model* Model, Player* p) : Unit(Model, p),
						   _buildingTime(0)
{
}

Building::~Building()
{
}

void	Building::Update(void)
{
	this->Unit::Update();
	Env*					env;
	std::list<Prod_Stack*>::iterator	it;

	env = Env::GetInstance();

	if (this->_productionList.empty() == false)
	{
		it = this->_productionList.begin();
		++_buildingTime;
		env->gameScreen->ProductionList.front()->Update(this);
		if (this->_buildingTime >= (*it)->prodDB->prodTime * MAX_TPS)
		{
			if (this->SpawnBuiltUnit((*it)->prodDB->prodID) == 1)
			{
				this->_buildingTime = 0;
				if ((_productionList.front())->nb < 1)
				{
					this->_productionList.pop_front();
					env->gameScreen->RefreshProductionList(this);
				}
				else
				{
					--(this->_productionList.front()->nb);
					this->_productionList.front()->LinkedButton->UpdateNB(_productionList.front()->nb + 1);
				}
			}
			else
				--_buildingTime;
		}
	}
}

void	Building::AddToBuildList(Production_DB* PDB)
{
	std::vector<Resource>::iterator		it;
	std::vector<Resource>::iterator		end;
	Prod_Stack*				PDS;

	Env*	env;

	env = Env::GetInstance();

	//#TODO Lock mutex game?
	it = PDB->resourcesList.begin();
	end = PDB->resourcesList.end();
	while (it != end)
	{
		if (this->GetPlayer()->GetResource((*it).model).qty < (*it).qty)
		{
			std::cout << "Not enough resources\n";
			return ;
		}
		++it;
	}

	it = PDB->resourcesList.begin();
	while (it != end)
	{
		this->GetPlayer()->AddResource((*it).model, -((*it).qty));
		++it;
	}

	if (this->_productionList.empty() == false)
	{
		if ((this->_productionList.back())->prodDB->prodID == PDB->prodID)
		{
			++(this->_productionList.back())->nb;
			env->gameScreen->ProductionList.back()->UpdateNB((this->_productionList.back()->nb) + 1);
		}
		else
		{
			//#TODO Check allocation!
			PDS = new Prod_Stack;
			PDS->prodDB = PDB;
			PDS->nb = 0;
			PDS->parent = this;
			PDS->LinkedButton = 0;
			this->_productionList.push_back(PDS);
		}
	}
	else
	{
		PDS = new Prod_Stack;
		PDS->prodDB = PDB;
		PDS->nb = 0;
		PDS->parent = this;
		PDS->LinkedButton = 0;
		this->_productionList.push_back(PDS);
	}
	env->gameScreen->RefreshProductionList(this);
}

int	Building::SpawnBuiltUnit(int builtID)
{
	Protocol_Manager*	pm;
	Database*		db;

	pm = Protocol_Manager::GetInstance();
	db = Database::GetInstance();

	if (this->GetPlayer()->GetPopulation() +
	    db->getUnitModel(builtID)->GetPopulation() > MAX_POPULATION)
		return 0;
	pm->prot.AddCmdGameCreateUnit(builtID, this->GetPosition().x, this->GetPosition().y, Game::GetInstance()->myID);
	return 1;
}

int	Building::GetProdPercent()
{
	std::list<Prod_Stack*>::iterator	it;

	it = this->_productionList.begin();
	return (this->_buildingTime / ((*it)->prodDB->prodTime * MAX_TPS) * 100);
}

bool	Building::IsInProd()
{
	if (_productionList.empty() == true)
		return (false);
	return (true);
}

std::list<Prod_Stack*>*	Building::GetProdList()
{
	return (&this->_productionList);
}
