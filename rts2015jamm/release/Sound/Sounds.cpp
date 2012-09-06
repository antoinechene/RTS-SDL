#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Sounds.h"

Sounds::Sounds(void): theme(_theme), alive(_alive), _theme(SOUND_OFF), _alive(true)
{

}

Sounds::~Sounds(void)
{
}

bool	Sounds::init(void)
{
	SDL_AudioSpec	desired;

	desired.freq = 44100;
	desired.format = AUDIO_U8;
	desired.channels = 1;
	desired.samples = 1024;
	desired.callback = &mixaudio;
	desired.userdata = NULL;
	if (SDL_OpenAudio(&desired, &(this->obtained)) != 0)
	{
		std::cout << "Fatal Audio Error: " << SDL_GetError() << std::endl;
		return true;
	}
	this->files.resize(15);
/*	if (this->LoadWAV("Resources/sounds/themeMenu.wav"))
		throw 801;
	if (this->LoadWAV("Resources/sounds/HOTEL_CALIFORNIA.wav"))
		throw 802;
	if (this->LoadWAV("Resources/sounds/themeB.wav"))
		throw 803;
	if (this->LoadWAV("Resources/sounds/themeC.wav"))
		throw 804;
	if (this->LoadWAV("Resources/sounds/themeEnd.wav"))
		throw 805;
	if (this->LoadWAV("Resources/sounds/nyan_original.wav"))
		throw 806;
	if (this->LoadWAV("Resources/sounds/bipGame.wav"))
		throw 807;
	if (this->LoadWAV("Resources/sounds/bipMenu1.wav"))
		throw 808;
	if (this->LoadWAV("Resources/sounds/bipMenu2.wav"))
		throw 809;
	if (this->LoadWAV("Resources/sounds/flip.wav"))
		throw 810;
	if (this->LoadWAV("Resources/sounds/put.wav"))
		throw 811;
	if (this->LoadWAV("Resources/sounds/brokenline.wav"))
		throw 812;
	if (this->LoadWAV("Resources/sounds/tetris.wav"))
		throw 813;
	if (this->LoadWAV("Resources/sounds/gameover.wav"))
		throw 814;
	if (this->LoadWAV("Resources/sounds/pause.wav"))
		throw 815;
*/	return false;
}

bool	Sounds::LoadWAV(const char* path)
{
	SDL_AudioSpec	soundfile;
	static int	key = 0;

	if (SDL_LoadWAV(path, &soundfile, &(this->sounddata), &(this->soundlength)) == NULL)
	{
		std::cout << "Loading WAV file error : " << SDL_GetError() << std::endl;
		return true;
	}
	if (SDL_BuildAudioCVT(&(this->files[key]),
			      soundfile.format,
			      soundfile.channels,
			      soundfile.freq,
			      this->obtained.format,
			      this->obtained.channels,
			      this->obtained.freq) < 0)
	{
		std::cout << "Error: Failed build audio coversion struct"<< std::endl;
		return true;
	}
	this->files[key].buf = new Uint8[this->soundlength *this->files[key].len_mult];
	if (this->files[key].buf == 0)
		throw 800;
	this->files[key].len = this->soundlength;
	memcpy(this->files[key].buf, this->sounddata, this->soundlength);
	if (SDL_ConvertAudio(&(this->files[key])) != 0)
	{
		std::cout << "Convert Audio Error: " << SDL_GetError() << std::endl;
		return true;
	}
	SDL_FreeWAV(this->sounddata);
	++key;
	return false;
}

void	mixaudio(void*, Uint8* stream, int len)
{
	Uint32		tocopy;
	Sounds*		sounds;

	sounds = Sounds::GetInstance();
	if (sounds->soundlength - sounds->soundpos > (unsigned int)len)
		tocopy = len;
	else
		tocopy = sounds->soundlength - sounds->soundpos;
	memcpy(stream, sounds->sounddata + sounds->soundpos, tocopy);
	sounds->soundpos += tocopy;
}

void	Sounds::play_sound(int key)
{
	SDL_LockAudio();
	pthread_mutex_lock(&(this->mutex));
	if (this->theme == SOUND_OFF)
	{
		if (this->sounddata == NULL)
			this->add_track(key);
		else if ((unsigned int)this->files[key].len_cvt > this->soundlength - this->soundpos)
		{
			Uint32				tocopy;
			std::list<Track*>::iterator	it;
			std::list<Track*>::iterator	end;
			Track*				track;

			tocopy = this->files[key].len_cvt;
			SDL_MixAudio(this->sounddata + this->soundpos, this->files[key].buf,
				     this->soundlength - this->soundpos, SDL_MIX_MAXVOLUME);
			tocopy -= (this->soundlength - this->soundpos);
			it = this->tracks_list.begin();
			end = this->tracks_list.end();
			while (tocopy > 0)
			{
				if (it == end)
				{
					track = new Track();
					if (track == 0)
						throw 500;
					track->data = new Uint8[tocopy];
					if (track->data == 0)
						throw 501;
					memcpy(track->data, this->files[key].buf +
					       (this->files[key].len_cvt - tocopy), tocopy);
					track->len = tocopy;
					this->tracks_list.push_back(track);
					tocopy = 0;
				}
				else
					tocopy = this->add_sound_to_track((*it),
			this->files[key].buf + (this->files[key].len_cvt - tocopy), tocopy);
				++it;
			}
		}
	}
	else if ((unsigned int)this->files[key].len_cvt > this->soundlength - this->soundpos)
	{
		Uint32	tocopy;\

		tocopy = this->files[key].len_cvt;
		SDL_MixAudio(this->sounddata + this->soundpos, this->files[key].buf,
			     this->soundlength - this->soundpos, SDL_MIX_MAXVOLUME);
		tocopy -= (this->soundlength - this->soundpos);
		while (tocopy > 0)
		{
			tocopy = this->add_sound_to_track(this->add_track(this->theme),
							  this->files[key].buf +
							  (this->files[key].len_cvt - tocopy),
							  tocopy);
		}
	}
	else
	{
		SDL_MixAudio(this->sounddata + this->soundpos, this->files[key].buf,
			     this->files[key].len_cvt, SDL_MIX_MAXVOLUME);
	}
	pthread_mutex_unlock(&(this->mutex));
	SDL_UnlockAudio();
}

void	Sounds::set_theme(int key)
{
	this->_theme = key;
	pthread_mutex_lock(&(this->mutex));
	while (!this->tracks_list.empty())
	{
		delete[] this->tracks_list.back()->data;
		this->tracks_list.pop_back();
	}
	this->new_theme = true;
	pthread_mutex_unlock(&(this->mutex));
}

void	Sounds::start_audio_player(void)
{
	if (pthread_mutex_init(&(this->mutex), NULL) != 0)
		return;
	pthread_create(&(this->thread), NULL, AudioPlayer, this);
}

void	Sounds::stop_audio_player(void)
{
	this->_alive = false;
	pthread_join(this->thread, NULL);
	pthread_mutex_destroy(&(this->mutex));
}

void	Sounds::pause_audio_player(void)
{
	SDL_LockAudio();
}

void	Sounds::unpause_audio_player(void)
{
	SDL_UnlockAudio();
}

Track*	Sounds::add_track(int key)
{
	Track*	track;

	try
	{
		track = new Track();
		if (track != 0)
		{
			track->data = new Uint8[this->files[key].len_cvt];
			if (track->data == 0)
				return 0;
			memcpy(track->data, this->files[key].buf, this->files[key].len_cvt);
				track->len = this->files[key].len_cvt;
			this->tracks_list.push_back(track);
		}
	}
	catch (...)
	{
		std::cout << "Fatal error in thread sound: add_track" << std::endl;
		return 0;
	}
	return track;
}

Uint32	Sounds::add_sound_to_track(Track* dst, Uint8* src, Uint32 len)
{
	if (dst->len < len)
	{
		Uint32	rest;

		rest = len - dst->len;
		SDL_MixAudio(dst->data, src, dst->len, SDL_MIX_MAXVOLUME);
		return rest;
	}
	else
	{
		SDL_MixAudio(dst->data, src, len, SDL_MIX_MAXVOLUME);
		return 0;
	}
}

Track*	Sounds::get_track(void)
{
	pthread_mutex_lock(&(this->mutex));
	if (this->tracks_list.empty())
	{
		if (this->theme != SOUND_OFF)
		{
			Track*	track;

			track = this->add_track(this->theme);
			if (track != NULL)
				this->tracks_list.pop_front();
			pthread_mutex_unlock(&(this->mutex));
			return track;
		}
		else
		{
			pthread_mutex_unlock(&(this->mutex));
			return NULL;
		}
	}
	else
	{
		Track*	track;

		track = this->tracks_list.front();
		this->tracks_list.pop_front();
		pthread_mutex_unlock(&(this->mutex));
		return track;
	}
	pthread_mutex_unlock(&(this->mutex));
	return NULL;
}

void	Sounds::play_track(Track* track)
{
	this->new_theme = false;
	this->soundpos = 0;
	this->soundlength = track->len;
	this->sounddata = track->data;
	SDL_PauseAudio(0);
	while (this->soundpos < this->soundlength && this->new_theme == false && this->alive == true)
		SDL_Delay(10);
	this->sounddata = NULL;
	SDL_PauseAudio(1);
	delete[] track->data;
	delete track;
}

void*	AudioPlayer(void*)
{
	Sounds*		sounds;
	Track*		track;

	try
	{
		sounds = Sounds::GetInstance();
		while (sounds->alive == true)
		{
			track = sounds->get_track();
			if (track != NULL)
				sounds->play_track(track);
			SDL_Delay(100);
		}
	}
	catch (int e)
	{
		std::cout << "Fatal error " << e << " in sound thread" << std::endl;
	}
	catch (...)
	{
		std::cout << "Fatal error in sound thread" << std::endl;
	}
	return NULL;
}

