#ifndef	__HUDEVENTSCALLBACKS_H__
#define	__HUDEVENTSCALLBACKS_H__

#include "../ID_glibrary.h"

class	Game_Screen;
class	Light_Drawable;
class	Unit;

void	Selector_CbClickDown(ID::Event* e, Game_Screen* gs);
void	Selector_CbClickUp(ID::Event* e, Game_Screen* gs);
void	Selector_CbMotion(ID::Event* e, Game_Screen* gs);
void	Order_CbClickDown(ID::Event* e, Light_Drawable* u);
void	Order_CbClickDown(ID::Event* e, Unit* u);
void	Order_CbMotion(ID::Event* e);

void	MapScrolling_CbMotion(ID::Event* e, Game_Screen* _gs);
void	GetSelection(ID::GRect*);

#endif
