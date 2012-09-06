#ifndef	__ENV_H__
#define	__ENV_H__

#include "Singleton.hpp"
#include <pthread.h>

namespace	ID
{
	class	Window;
};

class	Abstract_Screen;
class	Main_Screen;
class	Host_Screen;
class	Join_Screen;
class	Option_Screen;
class	Credit_Screen;
class	Lobby_Screen;
class	Game_Screen;

class	Env : public Singleton<Env>
{
	friend class	Singleton<Env>;
	public:
			Env();
			~Env();
		int	Init(ID::Window* window);
		void	SwitchScreen(Abstract_Screen* as);

	public:
		bool		alive;
		char		nickname[21];
		int		height;
		int		width;
		pthread_mutex_t	dataMutex;
		pthread_mutex_t	displayMutex;

		ID::Window*		window;
		Abstract_Screen*	focusScreen;
		Main_Screen*		mainScreen;
		Host_Screen*		hostScreen;
		Join_Screen*		joinScreen;
		Option_Screen*		optionScreen;
		Credit_Screen*		creditScreen;
		Lobby_Screen*		lobbyScreen;
		Game_Screen*		gameScreen;
};

#endif
