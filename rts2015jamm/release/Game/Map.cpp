#include "Map.h"
#include "Scripts/Script.hpp"
#include "Data_Access.h"
#include <iostream>

Map::Map() : //buf(2048, true),
	     hash(0),
	     w(0),
	     h(0),
	     defaultBgId(0),
	     countTeams(0),
	     teams(NULL),
	     minimap(NULL),
	     cases(NULL),
	     countInitialPos(0),
	     initialPos(NULL),
	     commonScripts(NULL),
	     globalScripts(NULL),
	     filename(NULL)
{
	this->colorsPalette[0] = ID::COLOR_RED.ConvertToInt();
	this->colorsPalette[1] = ID::COLOR_BLUE.ConvertToInt();
	this->colorsPalette[2] = ID::COLOR_ORANGE.ConvertToInt();
	this->colorsPalette[3] = ID::COLOR_SILVER.ConvertToInt();

	this->_colorsPalette[0] = ID::COLOR_RED;
	this->_colorsPalette[1] = ID::COLOR_BLUE;
	this->_colorsPalette[2] = ID::COLOR_ORANGE;
	this->_colorsPalette[3] = ID::COLOR_SILVER;
}

Map::~Map()
{
	this->__ClearMap();
}

int	Map::Load(char* filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("OPEN");
		return -1;
	}
	this->filename = filename;
	this->__ClearMap();
	std::cout << "Reading...\n";
	if (read(fd, &(this->hash), 13) == 13)
	{
		std::cout << "Hash:		" << std::hex << this->hash << '\n';
		std::cout << "W:		" << std::dec << this->w << '\n';
		std::cout << "H:		" << this->h << '\n';
		std::cout << "defaultBgId:	" << std::hex << this->defaultBgId << '\n';
		std::cout << "countTeams:	" << std::dec << (int)this->countTeams << '\n';
	}
	else
		return -1;

	if (this->__ParseTeams(fd) == -1)
		return -1;
	if (this->__ParseCases(fd) == -1)
		return -1;
	if (this->__ParseInitialPos(fd) == -1)
		return -1;

	std::cout << "\nEnd Map::Load()\n";
	if (close(fd))
		return -1;
	return 0;
}

void	Map::__ClearMap()
{
	this->hash = 0;
	this->w = 0;
	this->h = 0;
	this->defaultBgId = 0;
	this->countInitialPos = 0;

	while (this->countTeams > 0)
	{
		--this->countTeams;
		while (this->teams[this->countTeams].countPlayers > 0)
		{
			--this->teams[this->countTeams].countPlayers;
//			delete[] this->teams[this->countTeams]
//				.players[this->teams[this->countTeams].countPlayers]
//				.scripts;
		}
		delete[] this->teams[this->countTeams].players;
//		delete[] this->teams[this->countTeams].scripts;
//Les Scripts sont pour le moment out
	}
	delete[] this->teams;
	delete[] this->minimap;
	delete[] this->cases;
	delete[] this->initialPos;
	delete[] this->commonScripts;
	delete[] this->globalScripts;

	this->teams = NULL;
	this->minimap = NULL;
	this->cases = NULL;
	this->initialPos = NULL;
	this->commonScripts = NULL;
	this->globalScripts = NULL;
}

int	Map::__ParseTeams(int fd)
{
	int	i;

	this->teams = new Team[this->countTeams];
	if (this->teams != NULL)
	{
		i = 0;
		while (i < this->countTeams)
		{
			if (read(fd, &(this->teams[i]), 22) == 22)
			{
				std::cout << "Team->name:		" << this->teams[i].name << '\n';
				std::cout << "Team->countPlayers:	" << (int)this->teams[i].countPlayers << '\n';
				if (this->__ParseTeamPlayers(fd, i) == -1)
					return -1;
			}
			else
			{
				std::cerr << "Error LoadMap: read teams\n";
				return -1;
			}
			if (read(fd, &(this->teams[i].scripts), 4) == 4)
			{
				std::cout << "Team->script:	" << this->teams[i].scripts << '\n';
			}
			else
			{
				std::cerr << "Error LoadMap: read team's script\n";
				return -1;
			}
			++i;
		}
	}
	else
	{
		std::cerr << "Error LoadMap: allocation teams\n";
		return -1;
	}
	return 0;
}

int	Map::__ParseTeamPlayers(int fd, int i)
{
	int	t;

	this->teams[i].players = new Player[this->teams[i].countPlayers];
	if (this->teams[i].players == NULL)
	{
		std::cerr << "Error LoadMap: allocation team's players\n";
		return -1;
	}
	if (read(fd, &(this->teams[i].players[0]),
		 sizeof(Player) * this->teams[i].countPlayers) == (ssize_t)sizeof(Player) *
								  this->teams[i].countPlayers)
	{
		t = 0;
		while (t < this->teams[i].countPlayers)
		{
			std::cout << "Team->Player->name:	" << this->teams[i].players[t].name << '\n';
			std::cout << "Team->Player->color:	" << std::dec << (int)this->teams[i].players[t].color << '\n';
			std::cout << "Team->Player->fIP:	" << std::dec << (int)this->teams[i].players[t].forceInitialPos << '\n';
			std::cout << "Team->Player->fR:	" << std::dec << (int)this->teams[i].players[t].forceRace << '\n';
			std::cout << "Team->Player->script:	" << std::hex << this->teams[i].players[t].scripts << '\n';
			++t;
		}
	}
	else
	{
		std::cerr << "Error LoadMap: read team's players\n";
		return -1;
	}
	return 0;
}

int	Map::__ParseCases(int fd)
{
	Data_Access*	d;
	int		i;

	d = Data_Access::GetInstance();
	this->cases = new Case[this->w * this->h];
	if (this->cases!= NULL)
	{
		i = 0;
		while (i < this->w * this->h)
		{
			if (read(fd, &(this->cases[i]), 4) == 4)
			{
				this->cases[i].animation.SetSpriteAnime(d->GetMapPicture(this->cases[i].ID));
				if (this->cases[i].animation.GetSpriteAnime() == NULL)
				{
					std::cerr << "Error LoadMap: unreferenced ID " << this->cases[i].ID << "\n";
					return -1;
				}
//				std::cout << "Case[" << i << "]:	" << std::dec << this->cases[i].ID << '\n';
			}
			else
			{
				std::cerr << "Error LoadMap: read cases\n";
				return -1;
			}
			++i;
		}
	}
	else
	{
		std::cerr << "Error LoadMap: allocation cases\n";
		return -1;
	}
	return 0;
}

int	Map::__ParseInitialPos(int fd)
{
	int	i;

	if (read(fd, &(this->countInitialPos), 1) == 1)
	{
		std::cout << "countInitialPos:		" << std::dec << (int)this->countInitialPos << '\n';
	}
	else
	{
		std::cerr << "Error LoadMap: read countInitialPos\n";
		return -1;
	}

	this->initialPos = new InitialPosition[this->countInitialPos];
	if (this->initialPos != NULL)
	{
		if (read(fd,
			 this->initialPos,
			 sizeof(InitialPosition) * this->countInitialPos) != (ssize_t)sizeof(InitialPosition) * this->countInitialPos)
		{
			std::cerr << "Error LoadMap: read initialPos\n";
			return -1;
		}
		i = 0;
		while (i < this->countInitialPos)
		{
			std::cout << "initialPos[" << i << "]:	(" << this->initialPos[i].x << ", " << this->initialPos[i].y << ", " << (int)this->initialPos[i].team << ")\n";
			++i;
		}
	}
	else
	{
		std::cerr << "Error LoadMap: allocation initialPos\n";
		return -1;
	}
	return 0;
}
void	Case::AddUnit(Unit* u)
{
	this->units.push_back(u);
}

void	Case::DelUnit(Unit* u)
{
	std::list<Unit*>::iterator	it;
	std::list<Unit*>::iterator	end;

	it = this->units.begin();
	end = this->units.end();
	while (it != end)
	{
		if (*it == u)
		{
			this->units.erase(it);
			return ;
		}
		++it;
	}
}

