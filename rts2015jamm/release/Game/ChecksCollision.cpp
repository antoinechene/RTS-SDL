#include "ChecksCollision.h"
#include "../Constantes.h"
#include "Math_tools.h"

bool	CheckCollision_RectRect(ID::Rect* A, ID::Rect* B)
{
	int	leftA;
	int	rightA;
	int	topA;
	int	bottomA;
	int	leftB;
	int	rightB;
	int	topB;
	int	bottomB;

	leftA = A->x;
	rightA = A->x + A->w;
	topA = A->y;
	bottomA = A->y + A->h;

	leftB = B->x;
	rightB = B->x + B->w;
	topB = B->y;
	bottomB = B->y + B->h;

	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;
	return true;
}

bool	CheckCollision_RectCircle(ID::Rect* R, ID::Circle* C)
{
	ID::Point	cDist;
	int		cornerDist;
	int		halfW;
	int		halfH;

	halfW = R->w >> 1;
	halfH = R->h >> 1;
	if (C->center.x >= R->x)
		cDist.x = abs(C->center.x - R->x - halfW);
	else
		cDist.x = abs(R->x - C->center.x - halfW);
	if (C->center.y >= R->y)
		cDist.y = abs(C->center.y - R->y - halfH);
	else
		cDist.y = abs(R->y - C->center.y - halfH);

	if (cDist.x > halfW + C->radius ||
	    cDist.y > halfH + C->radius)
		return false;

	if (cDist.x <= halfW ||
	    cDist.y <= halfH)
		return true;

	cornerDist = pow<int>(cDist.x, 2) +
		     pow<int>(cDist.y, 2);
	return (cornerDist < C->radius * C->radius);
}
