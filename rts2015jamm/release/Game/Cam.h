#ifndef __CAM_H__
#define __CAM_H__

#include "../ID_glibrary.h"
#include "../Singleton.hpp"
#include "../Chrono.hpp"
#include <SDL/SDL.h>
#include <pthread.h>

void	__LD__CbClickDown(ID::Control* _ld, ID::Event* e, void* _gs);
void	__LD__CbClickUp(ID::Control* _ld, ID::Event* e, void* _gs);
void	__LD__CbMotion(ID::Control* _ld, ID::Event* e, void* _gs);

class	Map_Drawable;
class	Map;
class	Light_Drawable;

class	Cam : public Singleton<Cam>
{
	friend class	Singleton<Cam>;
	public:
			Cam();
			~Cam();
		int	Init(ID::Window*);
		int	Init(ID::Drawable*);
		void	SetCam(int x_c, int y_c);
		void	MoveCam(int x_c, int y_c);
		void	Refresh(void);
		void	RefreshScrolling(void);
		void	SetMapData(Map*);
		int	InitMapDrawable(void);
		void	InvalidateCases(float x, float y, int size);
		uint32_t	GetOffsetX(void);
		uint32_t	GetOffsetY(void);
		uint32_t	GetRealXPixel(uint32_t x);
		uint32_t	GetRealYPixel(uint32_t y);
		double		GetRealXCase(uint32_t x);
		double		GetRealYCase(uint32_t y);
		inline Light_Drawable*	GetCases()
		{ return this->cases; }
		inline Map_Drawable*	GetMapDrawable()
		{ return this->mapDrawable; }
		inline int	GetCameraWidth()
		{ return this->cameraWidth; }
		inline int	GetCameraHeight()
		{ return this->cameraHeight; }
		inline int	GetCameraWidthRayon()
		{ return this->cameraWidthRayon; }
		inline int	GetCameraHeightRayon()
		{ return this->cameraHeightRayon; }
		inline int	GetX()
		{ return this->__x; }
		inline int	GetY()
		{ return this->__y; }
		inline int	GetMapWidth()
		{ return this->mapWidth; }
		inline int	GetMapHeight()
		{ return this->mapHeight; }
		inline int	LockMutex()
		{ return pthread_mutex_lock(&(this->__camMutex)); }
		inline int	UnlockMutex()
		{ return pthread_mutex_unlock(&(this->__camMutex)); }
		inline int	TrylockMutex()
		{ return pthread_mutex_trylock(&(this->__camMutex)); }
		bool		b_left;
		bool		b_right;
		bool		b_top;
		bool		b_down;

	private:
		int		cameraWidth;
		int		cameraHeight;
		int		cameraWidthRayon;
		int		cameraHeightRayon;
		int		mapHeight;
		int		mapWidth;
		int		__x;
		int		__y;
		int		__nX;
		int		__nY;
		Map_Drawable*	mapDrawable;
		Light_Drawable*	cases;
		int		nb_case_screen;
		bool		__toRefresh;
		pthread_mutex_t	__camMutex;
		Chrono		__chrono;
};

#endif
