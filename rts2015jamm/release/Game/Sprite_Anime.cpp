#include "Sprite_Anime.h"

Sprite_Anime::Sprite_Anime(uint32_t ID,
			   char* filename,
			   uint16_t frequencyTick,
			   uint16_t nb_pict) : caract(NULL),
					       rects(NULL),
					       sprite(NULL),
					       ID(ID),
					       frequencyTick(frequencyTick),
					       nb_pict(nb_pict)
{
	this->rects = new ID::Rect[nb_pict];
	if (this->rects == NULL)
		throw 269;
	this->sprite = new ID::Surface(filename);
	this->sprite->SetColorKey(SDL_SRCCOLORKEY, 0xFFFF00FF);
	if (this->sprite == NULL)
		throw 270;
}

Sprite_Anime::~Sprite_Anime()
{
	delete[] this->rects;
	delete this->caract;
}

uint32_t	Sprite_Anime::GetID()
{
	return (this->ID);
}

uint16_t	Sprite_Anime::GetNbPict()
{
	return (this->nb_pict);
}

uint16_t	Sprite_Anime::GetFrequency()
{
	return this->frequencyTick;
}

int	Sprite_Anime::BlitPict(int x, int y, ID::Surface* dest)
{
	this->sprite->BlitSurface(&(this->rects[0]), dest, x, y);
	return (0);
}

ID::Rect*	Sprite_Anime::GetRects()
{
	return (this->rects);
}

ID::Surface*	Sprite_Anime::GetSprite()
{
	return (this->sprite);
}

ID::Color	Sprite_Anime::GetColor()
{
	return (this->color_case);
}

Caract_Picture::Caract_Picture()
{
	this->slow = 0;
	this->access = 1;
}

Caract_Picture::Caract_Picture(double slow, uint8_t a)
{
	this->slow = slow;
	this->access = a;
}

Caract_Picture::~Caract_Picture()
{

}

void	Caract_Picture::SetCaract(double slow, uint8_t a)
{
	this->slow = slow;
	this->access = a;
}

void	Caract_Picture::SetCaract(Caract_Picture* new_caract)
{
	this->slow = new_caract->slow;
	this->access = new_caract->access;
}

