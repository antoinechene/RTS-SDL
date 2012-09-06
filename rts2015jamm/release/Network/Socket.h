#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include <pthread.h>
#include <stdint.h>
#include "../Buffer.h"
#include "Protocol_Reader_Context.h"

class	Player;

class	Socket
{
 public:
	Socket(int fd, char* ip);
	~Socket(void);

	void	Accept(int* fd, char* ip);
	bool	start_com(void);
	void	stop_com(void);
	void	lockMutex(void);
	void	unlockMutex(void);
	int	Read(void);
	void	Write(void);

	bool	alive;
	Buffer	send;
	Buffer	read;
	char	ip[16];
	int	fd;
	Protocol_Reader_Context*	readContext;
	uint16_t			chunkID;
	Player*				player;

 private:
	pthread_mutex_t	mutex;
	pthread_t	thread;
};

#endif
