#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>
#include <vector>
#include <list>
#include <pthread.h>
#include "../Singleton.hpp"

enum	sound_type
{
	SOUND_THEME_MENU = 0,
/* Tout les sounds */
	SOUND_OFF
};

enum	song_type
{
	SONG_SHA = 0,
	SONG_WDY,
	SONG_DRS,
	SONG_OFF
};

enum	pos_sound
{
	SND_FRONT,
	SND_RIGHT,
	SND_LEFT,
	SND_DOWN
};

class	Sounds : public Singleton<Sounds>
{
	friend class Singleton<Sounds>;
public:
	bool		init(void);
	bool		LoadWAV(const char*);
	bool		LoadMUS(const char*);
	void		set_theme(int);
	void		stop_audio_player(void);
	bool		pause_audio_player(void);
	bool		unpause_audio_player(void);
	void		play_sound(int);
	void		play_distant_sound(int, int, int);
//	void		set_vol(int);
private:
	Sounds(void);
	~Sounds(void);
	void		set_vol_dist_sound(int, int);
	void		set_vol_song(int);

public:
	const int&			theme;
	const bool&			alive;
private:
	std::vector<Mix_Chunk*>		sounds;
	std::vector<Mix_Music*>		songs;
	int				volume;
	bool				new_theme;
	int				_theme;
	bool				_alive;
};


#endif

