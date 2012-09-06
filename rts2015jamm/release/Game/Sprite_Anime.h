#ifndef __SPRITE_ANIME_H__
#define __SPRITE_ANIME_H__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "../ID_glibrary.h"
#include "Game.h"

class	Sprite_Anime;

class	Caract_Picture
{
	public:
		Caract_Picture();
		Caract_Picture(double slow, uint8_t a);
		~Caract_Picture();
		void	SetCaract(double slow, uint8_t access);
		void	SetCaract(Caract_Picture* new_caract);
		double	slow;
		uint8_t	access;
};

class	Sprite_Anime
{
	public:
		Sprite_Anime(uint32_t ID, char* filename, uint16_t frequencyTick, uint16_t nb_pict);
		~Sprite_Anime();
		ID::Surface*	GetSprite();
		ID::Rect*	GetRects();
		ID::Color	GetColor();
		uint16_t	GetNbPict();
		uint16_t	GetFrequency();
		uint32_t	GetID();
		int		BlitPict(int x, int y, ID::Surface* dest);
		ID::Color	color_case;
		Caract_Picture*	caract;
	private:
		ID::Rect*	rects;
		ID::Surface*	sprite;
		uint32_t	ID;
		uint16_t	frequencyTick;
		uint16_t	nb_pict;
};

class	Animation
{
	public:
		Animation(Sprite_Anime* sprite = NULL) : __index(0),
							 __lastTick(0),
							 __sprite(sprite)
		{
		}
		~Animation()
		{
		}
		Animation&	operator=(const Animation& a)
		{
			this->__index = a.__index;
			this->__lastTick = a.__lastTick;
			this->__sprite = a.__sprite;
			return *this;
		}
		int	Animate(void)
		{
			Game*	gm;
			int	ret;

			gm = Game::GetInstance();
			ret = this->__index;
			if (this->__lastTick == 0)
			{
				this->__lastTick = gm->currentTick;
				return ret;
			}
			if (gm->currentTick - this->__lastTick > this->__sprite->GetFrequency())
			{
				++this->__index;
				if (this->__index >= this->__sprite->GetNbPict())
					this->__index = 0;
				this->__lastTick = gm->currentTick;
			}
			return ret;
		}
		inline void	SetSpriteAnime(Sprite_Anime* sprite)
		{
			this->__sprite = sprite;
		}
		inline Sprite_Anime*	GetSpriteAnime(void) const
		{
			return this->__sprite;
		}

	private:
		int		__index;
		uint32_t	__lastTick;
		Sprite_Anime*	__sprite;
};

#endif
