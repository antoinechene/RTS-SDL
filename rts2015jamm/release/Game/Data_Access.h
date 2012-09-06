#ifndef __DATA_ACCESS__
#define __DATA_ACCESS__

#include <png.h>
#include <vector>
#include <iostream>
#include "../ID_glibrary.h"
#include "Map.h"
#include "../Singleton.hpp"

class	Picture;
class	Sprite_Anime;
class	Caract_Picture;

class	Data_Access : public Singleton<Data_Access>
{
	friend class	Singleton<Data_Access>;
	public:
		int		Init();
		Sprite_Anime*	GetMapPicture(uint32_t ID);
		Sprite_Anime*	GetUnitPicture(uint32_t ID);
		Sprite_Anime*	GetPortraitPicture(uint32_t ID);
		int		LoadMapPicture(char* str, Caract_Picture* caract,
					       ID::Color* color, int ID);
		int		LoadUnitPicture(char* str, Caract_Picture* caract,
					        ID::Color* color, int ID);
		int		LoadPortraitPicture(char* str, Caract_Picture* caract,
					        ID::Color* color, int ID);
		int		LoadMapAnime(char* str, int w, int h,
					     int nb_frames, Caract_Picture* caract,
					     int frequency,
					     int ID);
		int		LoadUnitAnime(char* str, int w, int h,
					      int nb_frames, Caract_Picture* caract,
					      int frequency,
					      int ID);

	private:
				Data_Access();
				~Data_Access();
		Sprite_Anime*	__CreateSpritePicture(char* str,
						      Caract_Picture* caract,
						      ID::Color* color,
						      int ID);
		Sprite_Anime*	__CreateSpriteAnime(char* str, int x, int y, int w, int h,
						    int nb_frames, Caract_Picture* caract,
						    int frequency,
						    int ID);

	private:
		std::vector<Sprite_Anime*>	__mapPictures;
		std::vector<Sprite_Anime*>	__unitPictures;
		std::vector<Sprite_Anime*>	__portraitPictures;
};

#endif
