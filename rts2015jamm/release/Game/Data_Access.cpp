#include "Data_Access.h"
#include "Sprite_Anime.h"
#include "../Constantes.h"

Data_Access::Data_Access()
{
}

Data_Access::~Data_Access()
{
	std::vector<Sprite_Anime*>::iterator	itM;
	std::vector<Sprite_Anime*>::iterator	endM;
	std::vector<Sprite_Anime*>::iterator	itU;
	std::vector<Sprite_Anime*>::iterator	endU;

	itM = this->__mapPictures.begin();
	endM = this->__mapPictures.end();
	while (itM != endM)
	{
		delete *itM;
		++itM;
	}
	itU = this->__unitPictures.begin();
	endU = this->__unitPictures.end();
	while (itU != endU)
	{
		delete *itU;
		++itU;
	}
}

int	Data_Access::Init()
{
	Caract_Picture	caract;
	ID::Color	color(50, 54, 67, 0);

	caract.SetCaract(1.00, 1);
	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 0;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/mud.bmp", &caract, &color, 1))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/mud1.bmp", &caract, &color, 2))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/mud2.bmp", &caract, &color, 3))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/mud3.bmp", &caract, &color, 4))
		return -1;
	color.r = 202;
	color.g = 180;
	color.b = 95;
	color.a = 0;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/bridgevl.bmp", &caract, &color, 5))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/bridgevld.bmp", &caract, &color, 6))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/bridgevr.bmp", &caract, &color, 7))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/bridgevrd.bmp", &caract, &color, 8))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/plateformebd.bmp", &caract, &color, 9))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/plateformebg.bmp", &caract, &color, 10))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/plateformehd.bmp", &caract, &color, 11))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/plateformehg.bmp", &caract, &color, 12))
		return -1;
	color.r = 113;
	color.g = 71;
	color.b = 71;
	color.a = 0;

	caract.SetCaract(1.00, 0);
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/rochebd.bmp", &caract, &color, 13))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/rochebg.bmp", &caract, &color, 14))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/rochehd.bmp", &caract, &color, 15))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/rochehg.bmp", &caract, &color, 16))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/pierreh.bmp", &caract, &color, 22))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/pierreh2.bmp", &caract, &color, 23))
		return -1;
	color.r = 255;
	color.g = 255;
	color.b = 0;
	color.a = 0;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/sable.bmp", &caract, &color, 17))
		return -1;
	caract.SetCaract(1.00, 0);
	color.r = 0;
	color.g = 0;
	color.b = 255;
	color.a = 0;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/water.bmp", &caract, &color, 0))
		return -1;
	caract.SetCaract(1.00, 1);
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/waterdown.bmp", &caract, &color, 19))
		return -1;
	caract.SetCaract(1.00, 1);
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/waterup.bmp", &caract, &color, 20))
		return -1;
	color.r = 200;
	color.g = 200;
	color.b = 255;
	color.a = 0;

	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/glace.bmp", &caract,
	&color, 22))
		return -1;
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/neige.bmp", &caract,
	&color, 23))
		return -1;
	color.r = 30;
	color.g = 255;
	color.b = 30;
	color.a = 0;

	caract.SetCaract(1.00, 0);
	if (-1 == this->LoadMapPicture((char*)"Resources/Sprites/Maps/buisson.bmp",
	    &caract, &color, 21))
		return -1;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 0;
	if (this->LoadUnitAnime((char*)"Resources/Sprites/Units/1.bmp", 256, 128, 2,
				 &caract, MAX_TPS, 1) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/2.bmp",
				 &caract, &color, 2) == -1)
		return -1;
	if (this->LoadUnitAnime((char*)"Resources/Sprites/Units/3.bmp", 128, 32, 4,
				 &caract, MAX_TPS / 5, 3) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/4.bmp",
				 &caract, &color, 4) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/5.bmp",
				 &caract, &color, 5) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/6.bmp",
				 &caract, &color, 6) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/7.bmp",
				 &caract, &color, 7) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/8.bmp",
				 &caract, &color, 8) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/9.bmp",
				 &caract, &color, 9) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/10.bmp",
				 &caract, &color, 10) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/11.bmp",
				 &caract, &color, 11) == -1)
		return -1;
	if (this->LoadUnitPicture((char*)"Resources/Sprites/Units/12.bmp",
				 &caract, &color, 12) == -1)
		return -1;
	return 0;
}


Sprite_Anime*	Data_Access::GetMapPicture(uint32_t ID)
{
	std::vector<Sprite_Anime*>::iterator	it;
	std::vector<Sprite_Anime*>::iterator	end;

	it = this->__mapPictures.begin();
	end = this->__mapPictures.end();
	while (it != end)
	{
		if ((*it)->GetID() == ID)
			return *it;
		++it;
	}
	return NULL;
}

Sprite_Anime*	Data_Access::GetUnitPicture(uint32_t ID)
{
	std::vector<Sprite_Anime*>::iterator	it;
	std::vector<Sprite_Anime*>::iterator	end;

	it = this->__unitPictures.begin();
	end = this->__unitPictures.end();
	while (it != end)
	{
		if ((*it)->GetID() == ID)
			return *it;
		++it;
	}
	return NULL;
}

Sprite_Anime*	Data_Access::GetPortraitPicture(uint32_t ID)
{
	std::vector<Sprite_Anime*>::iterator	it;
	std::vector<Sprite_Anime*>::iterator	end;

	it = this->__portraitPictures.begin();
	end = this->__portraitPictures.end();
	while (it != end)
	{
		if ((*it)->GetID() == ID)
			return *it;
		++it;
	}
	return NULL;
}

int	Data_Access::LoadMapPicture(char* str,
				    Caract_Picture* caract,
				    ID::Color* color,
				    int ID)
{
	Sprite_Anime*	tmp;

	tmp = this->__CreateSpritePicture(str, caract, color, ID);
	if (tmp == NULL)
		return -1;
	this->__mapPictures.push_back(tmp);
	return 0;
}

int	Data_Access::LoadUnitPicture(char* str,
				     Caract_Picture* caract,
				     ID::Color* color,
				     int ID)
{
	Sprite_Anime*	tmp;

	tmp = this->__CreateSpritePicture(str, caract, color, ID);
	if (tmp == NULL)
		return -1;
	this->__unitPictures.push_back(tmp);
	return 0;
}

int	Data_Access::LoadPortraitPicture(char* str,
				     Caract_Picture* caract,
				     ID::Color* color,
				     int ID)
{
	Sprite_Anime*	tmp;

	tmp = this->__CreateSpritePicture(str, caract, color, ID);
	if (tmp == NULL)
		return -1;
	this->__portraitPictures.push_back(tmp);
	return 0;
}

int	Data_Access::LoadMapAnime(char* str,
				  int w, int h,
				  int nb_frames,
				  Caract_Picture* caract,
				  int frequency,
				  int ID)
{
	Sprite_Anime*	sprite;

	sprite = this->__CreateSpriteAnime(str, 0, 0, w, h, nb_frames, caract, frequency, ID);
	if (sprite == NULL)
		return -1;
	this->__mapPictures.push_back(sprite);
	return 0;
}

int	Data_Access::LoadUnitAnime(char* str,
				  int w, int h,
				  int nb_frames,
				  Caract_Picture* caract,
				  int frequency,
				  int ID)
{
	Sprite_Anime*	sprite;

	sprite = this->__CreateSpriteAnime(str, 0, 0, w, h, nb_frames, caract, frequency, ID);
	if (sprite == NULL)
		return -1;
	this->__unitPictures.push_back(sprite);
	return 0;
}

Sprite_Anime*	Data_Access::__CreateSpritePicture(char* str,
						   Caract_Picture* caract,
						   ID::Color* color,
						   int ID)
{
	Sprite_Anime*	tmp;

	tmp = new Sprite_Anime(ID, str, 0, 1);
	tmp->color_case = *color;
	tmp->caract = new Caract_Picture(caract->slow, caract->access);
	tmp->GetRects()[0].x = 0;
	tmp->GetRects()[0].y = 0;
	tmp->GetRects()[0].w = tmp->GetSprite()->w;
	tmp->GetRects()[0].h = tmp->GetSprite()->h;
	return tmp;
}

Sprite_Anime*	Data_Access::__CreateSpriteAnime(char* str,
						 int x, int y,
						 int w, int h,
						 int nb_frames,
						 Caract_Picture* caract,
						 int frequency,
						 int ID)
{
	Sprite_Anime*	new_anime;
	int		i;
	ID::Rect*	rects;

	new_anime = new Sprite_Anime(ID, str, frequency, nb_frames);
	new_anime->caract = new Caract_Picture(caract->slow, caract->access);
	rects = new_anime->GetRects();
	i = 0;
	while (i < nb_frames)
	{
		rects[i].x = x + i * (w / nb_frames);
		rects[i].y = y;
		rects[i].w = w / nb_frames;
		rects[i].h = h;
		++i;
	}
	return new_anime;
}

