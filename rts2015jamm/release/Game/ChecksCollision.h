#ifndef	__CHECKSCOLLISION_H__
#define	__CHECKSCOLLISION_H__

#include "../ID_glibrary.h"

bool	CheckCollision_RectRect(ID::Rect*, ID::Rect*);
bool	CheckCollision_RectCircle(ID::Rect* R, ID::Circle* C);

#endif
