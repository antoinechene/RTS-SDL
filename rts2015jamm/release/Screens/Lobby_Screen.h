#ifndef	__LOBBY_SCREEN_H__
#define	__LOBBY_SCREEN_H__

#include "../ID_glibrary.h"
#include "Abstract_Screen.hpp"
#include "../Game/Game.h"
#include <vector>

class	Lobby_Player_Line : public ID::Drawable
{
	public:
		Lobby_Player_Line(Player*);
		~Lobby_Player_Line();

	public:
		ID::Label		playerName;
		ID::Textbox		teamInput;
		ID::Textbox		colorInput;
		ID::Textbox		factionInput;
		ID::Button_Label	update;
		Player*			player;
};

class	Lobby_Screen : public Abstract_Screen
{
	public:
				Lobby_Screen();
				~Lobby_Screen();
		virtual void	ApplyEvent(ID::Event*);
		virtual void	Refresh();
		virtual void	Enter();
		virtual void	Leave();
		int		AddPlayer(Player*);
		void		DelPlayer(Player*);
		void		UpdatePlayerID(Player*);
		void		ClearPlayers(void);

	public:
		ID::Label			lobbyTitle;
		ID::Button_Label		startGame;
		ID::Button_Label		back;
		std::vector<Lobby_Player_Line*>	players;
};

#endif
