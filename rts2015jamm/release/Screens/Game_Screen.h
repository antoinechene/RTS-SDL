#ifndef	__GAME_SCREEN_H__
#define	__GAME_SCREEN_H__

#include "../ID_glibrary.h"
#include "Abstract_Screen.hpp"

class	Unit;
class	Unit_Model;
class	Building;
class	Prod_Stack;

class	HUD_DisplayInfo
{
	public:
		ID::Label	name;
		ID::Label	HP;
		ID::Label	ATK;
		ID::Label	DEF;
		ID::Surface	portrait;
		Unit*		lastUnit;
	public:
		HUD_DisplayInfo();
		~HUD_DisplayInfo();
		void	Update(Unit* unit);
		void	Show();
		void	Hide();
};

class	HUD_Selected
{
	public:
		ID::Button_Label	button;
		ID::Drawable		HPred;
		ID::Drawable		HPgreen;
		Unit*			LinkedUnit;
	public:
		HUD_Selected(int pos, Unit* unit);
		~HUD_Selected();
		void	SetPos(int x, int y);
		void	Show();
		void	Hide();
};

class	HUD_Production
{
	public:
		ID::Button_Label	InProd;
		ID::Label		nb;
		ID::GRect		prodTime;
	public:
		HUD_Production(int pos, Prod_Stack* unit);
		~HUD_Production();
		void	SetPos(int x, int y);
		void	Update(Building*);
		void	UpdateNB(int);
		void	Show();
		void	Hide();
};

class	Game_Screen : public Abstract_Screen
{
	public:
				Game_Screen();
				~Game_Screen();
		virtual	void	ApplyEvent(ID::Event*);
		virtual	void	Refresh();
		virtual	void	Enter();
		virtual	void	Leave();
			void	DisplayResult(int);
			void	RefreshMinimap();
			int	RefreshSelectedList();
			int	RefreshProductionList(Building*);
			void	Selected_Delete(int unit_token);
			void	ProdHUD(Unit* u, void (*callback)(ID::Control*, ID::Event*, void*));
			void	ScriptsHUD(Unit* u, void (*callback)(ID::Control*, ID::Event*, void*));
			void	UnitHUD();
			void	VoidHUD();

	public:
		ID::Drawable		HUD;
		std::vector<ID::Label*>	resources;
		ID::Label		population;
		ID::Label		result;
		ID::Button_Label		b1;
		ID::Button_Label		b2;
		ID::Button_Label		b3;
		ID::Button_Label		b4;
		ID::Button_Label		b5;
		ID::Button_Label		b6;
		ID::Button_Label		b7;
		ID::Button_Label		b8;
		ID::Button_Label		b9;
		ID::Surface			_minimap;
		ID::Drawable			minimap;
		bool				movingCam;
		bool				movingSelector;
		ID::Button_Label*		orderFocus;
		ID::GRect			selector;
		uint16_t			xSelector;
		uint16_t			ySelector;
		uint16_t			nXSelector;
		uint16_t			nYSelector;
		bool				refreshSelector;
		std::list<HUD_Selected*>	SelectedList;
		std::list<HUD_Production*>	ProductionList;
		HUD_DisplayInfo*		infos;
};

#endif
