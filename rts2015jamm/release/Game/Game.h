#ifndef	__GAME_H_
#define	__GAME_H_

#include "../Singleton.hpp"
#include "../ID_glibrary.h"
#include "../vector.h"

class	GameEvent;
class	Script;
class	Path;
class	Map;
class	Player;
class	Production_DB;
class	Light_Drawable;

class	Game : public Singleton<Game>
{
	friend class	Singleton<Game>;
	public:
		int	AddPlayer(char* name, int ID = -1);
		void	DelPlayer(int ID);
		int	UpdatePlayerFaction(uint8_t ID, uint8_t faction);
		int	UpdatePlayerID(uint8_t ID, uint8_t newID);
		int	InitLobby(char* filename, int ID);
		int	InitGame(void);
		int	StartGame(void);
		void	CleanAll(void);
		bool	ArePlayersReady(void);
		void	ResetPlayersReady(void);
		int	CheckEndGame(void);
		inline bool	IsGameRunning(void)
		{ return this->isGameStarted; }
		inline void	SetGameRunning(bool b)
		{ this->isGameStarted = b; }
		inline bool	IsInLobby(void)
		{ return this->isInLobby; }
		inline void	SetInLobby(bool b)
		{ this->isInLobby = b; }
		inline int	LockMutex(void)
		{ return pthread_mutex_lock(&(this->__gameMutex)); }
		inline int	TrylockMutex(void)
		{ return pthread_mutex_trylock(&(this->__gameMutex)); }
		inline int	UnlockMutex(void)
		{ return pthread_mutex_unlock(&(this->__gameMutex)); }
		inline Player*	GetMe()
		{
			return (this->players[this->myID]);
		}

	public:
		uint32_t		currentTick;
		ID::vector<GameEvent*>	gameEvents;
		ID::vector<Script*>	runningScripts;
		Script*			playerFocusScript;
		Production_DB*		playerFocusProd;
		Light_Drawable*		shadowProd;
		uint8_t			myID;
		bool			AmIHost;
		uint8_t			countPlayers;
		Player**		players;
//		ID::vector<Picture*>	orphanSprites;? In Unit maybe.
		bool			isInLobby;
		bool			isGameStarted;

		// Player data
		uint32_t	lastSelection;

	private:
			Game();
			~Game();
		int	__CheckInitialPos(Map* m);

	private:
		pthread_mutex_t	__gameMutex;
};

#endif
