#ifndef	__CONSTANTES_H__
#define	__CONSTANTES_H__

#include <SDL/SDL.h>

#define	VERSION		11.12.14.100

#define	PORT			(char*)"8877"
#define	MAX_FPS			90
#define	MAX_TPS			24
#define	CASE_SIDE_PIXELS	32
#define	DIV_CSP(x)		(x >> 5)
#define	MUL_CSP(x)		(x << 5)

#define	UI_WIDTH	1280
#define	UI_HEIGHT	1024
#define	UI_FLAGS	SDL_SWSURFACE
#define	UI_RMASK	0
#define	UI_GMASK	0
#define	UI_BMASK	0
#define	UI_AMASK	0

#define	DEFAULT_FONT_PATH	"Resources/sc.ttf"

#define	MAX_POPULATION	20
#define	MAX_GROUPS	10
#define	MAX_PLAYERS	4

#define	ABS(x)		((x >= 0) ? x : -(x))
#ifdef	__DEBUG
#define	DEBUG(x)	{std::cout << x;}
#define	std::cout	//std::cout
#else
#define	DEBUG(x)	{}
#endif

#endif
