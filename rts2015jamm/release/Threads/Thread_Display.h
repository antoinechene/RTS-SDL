#ifndef	__THREAD_DISPLAY_H__
#define	__THREAD_DISPLAY_H__

#include <pthread.h>
#include "../ID_glibrary.h"
#include "../Singleton.hpp"

class	Thread_Display : public Singleton<Thread_Display>
{
	friend class	Singleton<Thread_Display>;
	public:
		static void*	Run(void*);
		int		Init();
		void		Destroy(void);

	private:
				Thread_Display();
				~Thread_Display();
		bool		alive;
		pthread_t	idThread;
		ID::Window*	win;
};

#endif
