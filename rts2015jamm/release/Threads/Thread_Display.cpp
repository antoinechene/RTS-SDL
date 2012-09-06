#include "Thread_Display.h"
#include "../Env.h"
#include "../Chrono.hpp"
#include "../Game/Cam.h"
#include "../Screens/Abstract_Screen.hpp"
#include <sys/time.h>

Thread_Display::Thread_Display() : alive(true)
{
}

Thread_Display::~Thread_Display()
{
}

int	Thread_Display::Init()
{
	this->alive = true;
	if (pthread_create(&(this->idThread), NULL, this->Run, this) != 0)
		return -1;
	return 0;
}

void	Thread_Display::Destroy()
{
	this->alive = false;
	if (pthread_join(this->idThread, NULL) != 0)
	{
		std::cerr << "Error pthread_join Thread_Display" << std::endl;
	}
}

void*	Thread_Display::Run(void* _td)
{
	Thread_Display*		td;
	pthread_mutex_t*	m;
	uint32_t		interval;
	Env*			env;
	struct timeval		nextFrame;
	struct timeval		nextRefreshFPS;
	struct timeval		current;
	uint32_t		fps;
	Chrono			c;

	td = (Thread_Display*)_td;
	env = Env::GetInstance();
	m = &(env->displayMutex);

	fps = 0;
	interval = 1000000 / MAX_FPS;
	if (gettimeofday(&current, NULL) == 0)
	{
		nextFrame.tv_sec = current.tv_sec;
		nextFrame.tv_usec = current.tv_usec + interval;
		nextRefreshFPS.tv_sec = current.tv_sec;
		nextRefreshFPS.tv_usec = current.tv_usec;
	}

	while (td->alive == true)
	{
		if (gettimeofday(&current, NULL) == 0)
		{
			if (timercmp(&nextFrame, &current, <))
			{
				pthread_mutex_lock(m);
			//	c.SaveTime();
				if (env->focusScreen != NULL)
					env->focusScreen->Refresh();
			//	std::cout << "DispT=" << c.NewTime() << "\n";
				if (env->window->RefreshSurface() != 0)
					std::cerr << "Error TD::Window::RefreshSurface()" << std::endl;
			//	std::cout << "DispT2=" << c.NewTime() << "\n";
				env->window->Update(); //#TODO Retirer le 0 si necessaire
			//	std::cout << "DispT3=" << c.NewTime() << "\n";
				pthread_mutex_unlock(m);
				nextFrame.tv_sec = current.tv_sec;
				nextFrame.tv_usec = current.tv_usec + interval;
				++fps;
//				usleep(10);
			}
			if (timercmp(&nextRefreshFPS, &current, <))
			{
				nextRefreshFPS.tv_sec = current.tv_sec + 1;
				nextRefreshFPS.tv_usec = current.tv_usec;
				std::cout << "FPS: " << fps << std::endl;
				fps = 0;
			}
		}
	}
	return 0;
}

