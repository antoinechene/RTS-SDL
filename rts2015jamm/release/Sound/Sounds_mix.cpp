#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Sounds_mix.h"

Sounds::Sounds(void): theme(_theme), alive(_alive), _theme(SOUND_OFF), _alive(true)
{

}

Sounds::~Sounds(void)
{
}

bool	Sounds::init(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		std::cout << "Fatal Audio Error: " << SDL_GetError() << std::endl;
		return true;
	}
	Mix_AllocateChannels(2);
	if (this->LoadWAV("Resources/sounds/break.wav"))
		throw 801;
	if (this->LoadMUS("Resources/sounds/SHA.mp3"))
		throw 802;
	if (this->LoadMUS("Resources/sounds/WDY.mp3"))
		throw 803;
	if (this->LoadMUS("Resources/sounds/DRS.mp3"))
		throw 804;
	return false;
}

bool	Sounds::LoadWAV(const char* path)
{
	static int	key = 0;
	Mix_Chunk*	sound;

	sound = Mix_LoadWAV(path);
	if (sound == NULL)
	{
		std::cout << "Error LoadWAV: " << path << std::endl;
	}
	this->sounds.push_back(sound);
	++key;
	return false;
}

bool	Sounds::LoadMUS(const char* path)
{
	static int	key = 0;
	Mix_Music*	song;

	song = Mix_LoadMUS(path);
	if (song == NULL)
	{
		std::cout << "Error LoadMUS: " << path << std::endl;
	}
	this->songs.push_back(song);
	++key;
	return false;
}

void	Sounds::play_sound(int key)
{
	if (key >= SOUND_OFF || key < SOUND_THEME_MENU)
		return;
	if (Mix_Paused(1) == 0)
	{
		if (Mix_PlayChannel(1, this->sounds[key], 0) == -1)
		{
			std::cout << "faileuh" << this->sounds[key] << std::endl;
			return;
		}
	}
}

void	Sounds::set_theme(int key)
{
	if (key >= SONG_OFF || key < SONG_SHA)
		return;
	this->_theme = key;
	if (Mix_PausedMusic() != 1)
	{
		if (Mix_PlayMusic(this->songs[key], -1) == -1)
		{
			std::cout << "Faileuh pour la musiqueuh" << std::endl;
		}
	}
}

void	Sounds::stop_audio_player(void)
{
	std::vector<Mix_Chunk*>::iterator	itC;
	std::vector<Mix_Chunk*>::iterator	endC;
	std::vector<Mix_Music*>::iterator	itM;
	std::vector<Mix_Music*>::iterator	endM;

	itC = this->sounds.begin();
	endC = this->sounds.end();
	itM = this->songs.begin();
	endM = this->songs.end();

	while (itC != endC)
	{
		Mix_FreeChunk(*itC);
		++itC;
	}
	while (itM != endM)
	{
		Mix_FreeMusic(*itM);
		++itM;
	}
	this->_alive = false;
	Mix_CloseAudio();
}

bool	Sounds::pause_audio_player(void)
{
	if (Mix_PausedMusic() != 1)
	{
		Mix_PauseMusic();
		Mix_Pause(1);
		return (true);
	}
	return (false);
}

bool	Sounds::unpause_audio_player(void)
{
	if (Mix_PausedMusic() == 1)
	{
		Mix_ResumeMusic();
		Mix_Resume(1);
		return (true);
	}
	return (false);
}

void	Sounds::play_distant_sound(int key, int pos, int dist)
{
	this->set_vol_dist_sound(key, dist);
	if (pos == SND_FRONT)
	{
		if (!Mix_SetPosition(1, 0, 0))
		{
			std::cout << "Error SND_FRONT" << std::endl;
		}
	}
	else if (pos == SND_RIGHT)
	{
		if (!Mix_SetPosition(1, 90, 0))
		{
			std::cout << "Error SND_RIGHT" << std::endl;
		}
	}
	else if (pos == SND_LEFT)
	{
		if (!Mix_SetPosition(1, 270, 0))
		{
			std::cout << "Error SND_LEFT" << std::endl;
		}
	}
	else if (pos == SND_DOWN)
	{
		if (!Mix_SetPosition(1, 180, 0))
		{
			std::cout << "Error SND_DOWN" << std::endl;
		}
	}
	this->play_sound(key);
}

void	Sounds::set_vol_dist_sound(int key, int dist)
{
	int	vol;

	vol = MIX_MAX_VOLUME - dist;
	if (vol < 0)
		vol = 0;
	Mix_VolumeChunk(this->sounds[key], vol);
}
