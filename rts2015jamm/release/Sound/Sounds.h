#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>
#include <list>
#include <pthread.h>
#include "../Singleton.hpp"

enum	sound_type
{
	SOUND_THEME_MENU = 0,
	SOUND_THEME_A,
	SOUND_THEME_B,
	SOUND_THEME_C,
	SOUND_THEME_END,
	SOUND_THEME_NYAN,
	SOUND_BIP_GAME,
	SOUND_BIP_MENU_1,
	SOUND_BIP_MENU_2,
	SOUND_FLIP,
	SOUND_PUT,
	SOUND_BROKENLINE,
	SOUND_TETRIS,
	SOUND_GAMEOVER,
	SOUND_PAUSE,
	SOUND_OFF
};

struct	Track
{
	Uint8*	data;
	Uint32	len;
};

class	Sounds : public Singleton<Sounds>
{
	friend class Singleton<Sounds>;
public:
	bool		init(void);
	bool		LoadWAV(const char*);
	void		set_theme(int);
	void		start_audio_player(void);
	void		stop_audio_player(void);
	void		pause_audio_player(void);
	void		unpause_audio_player(void);
	Track*		get_track(void);
	void		play_sound(int);
	void		play_track(Track*);
private:
	Sounds(void);
	~Sounds(void);

	Track*		add_track(int);
	Uint32		add_sound_to_track(Track*, Uint8*, Uint32);
public:
	Uint32				soundlength;
	Uint32				soundpos;
	Uint8*				sounddata;
	const int&			theme;
	const bool&			alive;
private:
	std::vector<SDL_AudioCVT>	files;
	std::list<Track*>		tracks_list;
	SDL_AudioSpec			obtained;
	pthread_t			thread;
	pthread_mutex_t			mutex;
	bool				new_theme;
	int				_theme;
	bool				_alive;
};

void	mixaudio(void*, Uint8*, int);
void*	AudioPlayer(void*);

#endif

