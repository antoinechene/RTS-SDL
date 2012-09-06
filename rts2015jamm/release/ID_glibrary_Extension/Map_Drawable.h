#ifndef	__MAP_DRAWABLE_H__
#define	__MAP_DRAWABLE_H__

#include "../ID_glibrary.h"
#include <vector>

enum	LEVEL_GROUND_TYPE
{
	UNDER_BACKGROUND	= 0, // Exemple: Ecoulement de lave? XD
	BACKGROUND,		// Exemple: Image du sol
	ON_BACKGROUND,		// Exemple: Image de cadavre
	UNDER_MIDDLEGROUND,	// Exemple: Cercle de focus terrestre
	MIDDLEGROUND,		// Exemple: Unitees terrestre
	UNDER_HIGHGROUND,	// Exemple: Cercle de focus aerien
	HIGHGROUND,		// Exemple: Avion
	ON_HIGH_GROUND,		// Exemple: Barre de vie
	FOREGROUND		// Exemple: Nuage, skills
};

class	Map_Drawable : public ID::Drawable
{
	public:
		Map_Drawable(int level, uint16_t w, uint16_t h);
		~Map_Drawable();

		void	AddGround(ID::Drawable* d, int level);
		void	DelGround(ID::Drawable* d, int level);
		void	ClearGround(int level);
		std::vector<ID::Drawable*>*
			GetGround(int level);

		uint16_t	GetWidth(void) const
		{
			return this->_w;
		}
		uint16_t	GetHeight(void) const
		{
			return this->_h;
		}
		virtual int	RefreshToSurface(ID::Surface* s,
						 int x, int y);

		virtual int 	Click(ID::Event* event);
		virtual int 	DoubleClick(ID::Event* event);
		virtual int 	ClickDown(ID::Event* event);
		virtual int 	ClickUp(ID::Event* event);
		virtual int 	Enter(ID::Event* event);
		virtual int 	Leave(ID::Event* event);
		virtual int 	Motion(ID::Event* event);
		virtual int 	KeyUp(ID::Event* event);
		virtual int 	KeyDown(ID::Event* event);
		virtual int 	FocusEnter(ID::Event* event);
		virtual int 	FocusLeave(ID::Event* event);
		virtual int 	EnableChange(ID::Event* event);
		virtual int 	ScreenChange(ID::Event* event);
		virtual int 	Resize(ID::Event* event);
		virtual int 	Quit(ID::Event* event);
		virtual int 	UserEvent(ID::Event* event);

		virtual bool	_OnClick(ID::Event* event);
		virtual bool	_OnDoubleClick(ID::Event* event);
		virtual bool	_OnClickDown(ID::Event* event);
		virtual bool	_OnClickUp(ID::Event* event);
		virtual bool	_OnEnter(ID::Event* event);
		virtual bool	_OnLeave(ID::Event* event);
		virtual bool	_OnMotion(ID::Event* event);
		virtual bool	_OnKeyUp(ID::Event* event);
		virtual bool	_OnKeyDown(ID::Event* event);
		virtual bool	_OnFocusEnter(ID::Event* event);
		virtual bool	_OnFocusLeave(ID::Event* event);
		virtual bool	_OnEnableChange(ID::Event* event);
		virtual bool	_OnScreenChange(ID::Event* event);
		virtual bool	_OnResize(ID::Event* event);
		virtual bool	_OnQuit(ID::Event* event);
		virtual bool	_OnUserEvent(ID::Event* event);

	protected:
		std::vector< std::vector<ID::Drawable*> >	_grounds;

	private:
		int	__RefreshGrounds(int min, int max, ID::Surface* s, int x, int y);
		int	__RefreshBars(ID::Surface* s, int x, int y, int size, int hp, int maxHP);
};

#endif
