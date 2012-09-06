#ifndef	__MAP_H__
#define	__MAP_H__

#include "Sprite_Anime.h"
#include "../Constantes.h"
#include "../Singleton.hpp"
#include "../ID_glibrary.h"
#include "../Buffer.h"
#include <stdint.h>
#include <fcntl.h>
#include <list>

class	Script;
class	Unit;

class	Case
{
	public:
		void	AddUnit(Unit*);
		void	DelUnit(Unit*);

	public:
		uint32_t		ID;
		std::list<Unit*>	units;
		Animation		animation;
		//uint32_t		z;
};

class	Map : public Singleton<Map>
{
	friend class	Singleton<Map>;
	public:
	class	InitialPosition
	{
		public:
			uint16_t	x;
			uint16_t	y;
			unsigned char		team;
	};

	class	Player
	{
		public:
			char		name[21];
			unsigned char	color; // Sur une palette predefinie
			unsigned char	forceInitialPos;
			unsigned char	forceRace;
			Script*		scripts;
	};

	/*
	IDPLAYERS EST INCREMENTE AVEC TEAM, ce qui est une erreur...
	 */
	class	Team
	{
		public:
			char		name[21];
			unsigned char	countPlayers;
			Player*		players;
			Script*		scripts;
	};

	public:
		int	Load(char* filename);

	public:
		uint32_t		colorsPalette[MAX_PLAYERS];
		ID::Color		_colorsPalette[MAX_PLAYERS];
//		Buffer			buf;
		uint32_t		hash;
		uint16_t		w;
		uint16_t		h;
		uint32_t		defaultBgId;
		uint8_t			countTeams;
		Team*			teams;
		uint16_t*		minimap;
		Case*			cases;
		uint8_t			countInitialPos;
		InitialPosition*	initialPos;
		Script*			commonScripts;
		Script*			globalScripts;
		char*			filename;

	private:
			Map();
			~Map();
		void	__ClearMap(void);
		int	__ParseTeams(int fd);
		int	__ParseTeamPlayers(int fd, int i);
		int	__ParseCases(int fd);
		int	__ParseInitialPos(int fd);
};

#endif
