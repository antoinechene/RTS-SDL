#ifndef	__THREAD_GAMEPLAY_H__
#define	__THREAD_GAMEPLAY_H__

#include <pthread.h>
#include "../ID_glibrary.h"
#include "../Singleton.hpp"

class	Thread_Gameplay : public Singleton<Thread_Gameplay>
{
	friend class	Singleton<Thread_Gameplay>;
	public:
		static void*	Run(void*);
		int		Init(void);
		void		Destroy(void);

	private:
				Thread_Gameplay();
				~Thread_Gameplay();
		bool		alive;
		pthread_t	idThread;
};

#endif
