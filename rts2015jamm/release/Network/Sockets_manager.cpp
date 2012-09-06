#include "Sockets_manager.h"
#include "Socket.h"
#include "../Game/Game.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include <string.h>

Sockets_manager::Sockets_manager() : alive(false), hostSocket(NULL), listenSocket(NULL)
{
}

Sockets_manager::~Sockets_manager()
{
	this->alive = true;
	this->ClearSockets();
}
Socket*	Sockets_manager::ListenSocket(char* port)
{
	if (this->listenSocket == NULL)
	{
		int	e;

		e = this->__ListenSocket(port);
		if (e != 0)
		{
			std::cerr << "Error " << e << ": Trying to listen socket failed\n";
			return NULL;
		}
	}
	return this->listenSocket;
}

Socket*	Sockets_manager::ConnectSocket(char* port, char* ip)
{
	Socket*				skt;
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;

	it = this->clientSockets.begin();
	end = this->clientSockets.end();
	while (it != end)
	{
		if ((*it)->ip != NULL &&
		    strcmp((*it)->ip, ip) == 0)
			return NULL;
		++it;
	}
	try
	{
		skt = this->__GetClientSocket(port, ip);
	}
	catch (int e)
	{
		std::cerr << "Error " << e << ": Connection to socket failed" << std::endl;
		return NULL;
	}
	return skt;
}

int	Sockets_manager::AcceptFromHost(void)
{
	Socket*	skt;
	char	ip[16];

	skt = this->GetHostSocket();
	if (skt != NULL)
	{
		int		fdmax;
		fd_set		fdread;
		struct timeval	timeout;

		fdmax = skt->fd + 1;
		FD_ZERO(&fdread);
		FD_SET(skt->fd, &fdread);
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
		if (select(fdmax, &fdread, 0, 0, &timeout) == -1)
			return -1;
		if (FD_ISSET(skt->fd, &fdread))
		{
			skt->Accept(&fdmax, ip);
			if (fdmax != -1)
			{
				skt = NULL;
				while (skt == NULL)
					skt = new Socket(fdmax, ip);
				this->clientSockets.push_back(skt);
				return 1;
			}
		}
	}
	return 0;

}

std::vector<Socket*>*	Sockets_manager::GetSockets()
{
	return &(this->clientSockets);
}

void		Sockets_manager::SetHostSocket(Socket* host)
{
	this->hostSocket = host;
}

Socket*		Sockets_manager::GetHostSocket(void)
{
	return this->hostSocket;
}

Socket*		Sockets_manager::GetListenSocket(void)
{
	return this->listenSocket;
}
void*		Sockets_manager::Accept(void* _sm)
{
	(void)_sm;
/*	Sockets_manager*	sm;
	int			fdmax;
	fd_set			fdread;
	unsigned int		i;
	struct timeval		timeout;

	sm = (Sockets_manager*)_sm;
	while (!sm->alive)
	{
		fdmax = 1;
		FD_ZERO(&fdread);
		i = 0;
		while (i < sm->GetSockets()->size())
		{
			FD_SET(sm->GetSockets()->at(i)->fd, &fdread);
			if (sm->GetSockets()[i]->fd >= fdmax)
				fdmax = sm->GetSockets()[i]->fd + 1;
			i++;
		}
		timeout.tv_sec = 0;
		timeout.tv_usec = 50000;
		if (select(fdmax, &fdread, 0, 0, &timeout) > 0)
		{
			i = 0;
			while (i < sm->GetSockets()->size())
			{
				if (FD_ISSET(sm->GetSockets()[i]->fd,
					     &fdread))
					std::cout << "Accepting socket" << std::endl;
				i++;
			}
		}
	}*/
	return 0;
}

void	Sockets_manager::ClearSockets()
{
	std::cout << "ClearSockets()" << std::endl;
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;

	it = this->clientSockets.begin();
	end = this->clientSockets.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
	this->clientSockets.clear();
	if (Game::GetInstance()->AmIHost == true)
	{
		std::cout << "Host ";
		delete this->listenSocket;
	}
	this->listenSocket = NULL;
	this->hostSocket = NULL;
}

void	Sockets_manager::DeleteSocket(Socket* s)
{
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;

	it = this->clientSockets.begin();
	end = this->clientSockets.end();
	while (it != end)
	{
		if (*it == s)
		{
			if (s == this->GetHostSocket())
			{
				std::cout << "Host socket deleted\n";
				this->SetHostSocket(NULL);
			}
			delete *it;
			this->clientSockets.erase(it);
			return ;
		}
		++it;
	}
}

void	Sockets_manager::DeleteSocket(std::vector<Socket*>::iterator it)
{
	if (*it == this->GetHostSocket())
	{
		std::cout << "Host socket deleted\nHardcore reset.\n";
		this->SetHostSocket(NULL);
	}
	delete *it;
	this->clientSockets.erase(it);
}

int		Sockets_manager::__ListenSocket(char* port)
{
	struct addrinfo		hints;
	struct addrinfo*	res;
	int			fd;
	char			hostname[50];
	sockaddr_in		hostAddr;
	socklen_t		len;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (gethostname(hostname, 50) == -1)
		return -7;
	if (getaddrinfo(hostname, port, &hints, &res))
		return -1;
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd == -1)
	{
		freeaddrinfo(res);
		return -2;
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&fd, sizeof(fd)) < 0)
	{
		freeaddrinfo(res);
		close(fd);
		return -3;
	}
	if (bind(fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		perror("Bind: ");
		freeaddrinfo(res);
		close(fd);
		return -4;
	}
	freeaddrinfo(res);
	if (listen(fd, 11))
	{
		close(fd);
		return -5;
	}
	len = sizeof(hostAddr);
	if (getsockname(fd, (struct sockaddr*)&hostAddr, &len) == -1)
		return -8;
	this->listenSocket = new Socket(fd, inet_ntoa(hostAddr.sin_addr));
	if (this->listenSocket == NULL)
	{
		if (close(fd) == -1)
			std::cerr << "Error Sockets_manager: close FD " << fd << " failed.\n";
		return -6;
	}
	return 0;
}

Socket*	Sockets_manager::__GetClientSocket(char* port, char* ip)
{
	struct addrinfo		hints;
	struct addrinfo*	res;
	int			fd;
	Socket*			newSocket;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(ip, port, &hints, &res))
		throw 85;
	fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (fd == -1)
	{
		freeaddrinfo(res);
		close(fd);
		throw 86;
	}
	if (connect(fd, res->ai_addr, res->ai_addrlen) == -1)
	{
		freeaddrinfo(res);
		close(fd);
		throw 87;
	}
	freeaddrinfo(res);
	newSocket = new Socket(fd, ip);
	if (newSocket == NULL)
	{
		close(fd);
		return NULL;
	}
	this->clientSockets.push_back(newSocket);
	return newSocket;
}

