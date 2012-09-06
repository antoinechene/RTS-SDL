#include "Socket.h"
#include "Sockets_manager.h"
#include "Protocol_Manager.h"
#include "../Game/Game.h"
#include "../Game/Player.h"
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

/*
void*	communication(void* _sock)
{
	fd_set		fdread;
	fd_set		fdwrite;
	struct timeval	timeout;
	Socket*		sock;

	sock = (Socket*)_sock;
	while (sock->alive == true)
	{
		timeout.tv_usec = 1000;
		timeout.tv_sec = 0;
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_SET(sock->fd, &fdread);
		if (strlen(sock->send) > 0)
			FD_SET(sock->fd, &fdwrite);
		if (select(sock->fd + 1, &fdread, &fdwrite, NULL, &timeout) > 0)
		{
			if (FD_ISSET(sock->fd, &fdread))
			{
				if (sock->Read())
					return NULL;
			}
			if (FD_ISSET(sock->fd, &fdwrite))
				sock->Write();
		}
	}
	return NULL;
}
*/
Socket::Socket(int fd, char* _ip) : alive(false),
				    send(512, true),
				    read(512),
				    fd(fd),
				    readContext(NULL),
				    chunkID(0),
				    player(NULL)
{
	this->ip[0] = 0;
	if (_ip != NULL)
		strcpy(this->ip, _ip);
	if (pthread_mutex_init(&(this->mutex), NULL) != 0)
		return ;
	std::cout << "Socket(" << fd << ", " << _ip << ")" << std::endl;
}

Socket::~Socket()
{
	Protocol_Manager*	pm;
	Game*			gm;
	int			i;

	gm = Game::GetInstance();
	pm = Protocol_Manager::GetInstance();
	i = 0;
	while (i < gm->countPlayers)
	{
		if (gm->players[i] != NULL &&
		    gm->players[i]->GetSocket() == this)
		{
			if (gm->AmIHost == true)
			{
				if (gm->IsInLobby() == true)
					pm->prot.AddCmdLobbyLeave(i);
				else if (gm->IsGameRunning() == true)
				{
					//#TODO Ajouter command de leave game
				}
			}
			gm->DelPlayer(i);
			break ;
		}
		++i;
	}
	pthread_mutex_destroy(&(this->mutex));
	if (close(this->fd) == -1)
		std::cerr << "Error Socket: close FD " << this->fd << " failed." << std::endl;
	else
		std::cout << "Socket [" << this->fd << "] closed." << std::endl;
}

void	Socket::Accept(int* fd, char* ip)
{
	Sockets_manager*		sm;
	Socket*				skt;
	std::vector<Socket*>::iterator	it;
	std::vector<Socket*>::iterator	end;
	socklen_t			len;
	sockaddr_in			clientAddr;

	len = sizeof(clientAddr);
	*fd = ::accept(this->fd, (struct sockaddr*)&clientAddr, &len);
	if (ip != NULL)
		strncpy(ip, inet_ntoa(clientAddr.sin_addr), 16);
	if (*fd != -1)
	{
		sm = Sockets_manager::GetInstance();
		it = sm->GetSockets()->begin();
		end = sm->GetSockets()->end();
		skt = sm->GetListenSocket();
		if (strcmp(skt->ip, ip) == 0) // Avoid local connection
		{
			if (close(*fd) == -1)
				std::cerr << "Error Socket: close FD " << *fd << "failed." << std::endl;
			*fd = -1;
			return ;
		}
		while (it != end) // Avoid double connection from a client
		{
			if (strcmp((*it)->ip, ip) == 0)
			{
				if (close(*fd) == -1)
					std::cerr << "Error Socket: close FD " << *fd << "failed." << std::endl;
				*fd = -1;
				return ;
			}
			++it;
		}
	}
}

bool	Socket::start_com()
{
	this->alive = true;
/*	if (pthread_mutex_init(&(this->mutex), NULL) != 0 &&
	    pthread_create(&(this->thread), NULL, communication, this))
		return (true);
*/	return (false);
}

void	Socket::stop_com()
{
	this->alive = false;
	pthread_join(this->thread, NULL);
	pthread_mutex_destroy(&(this->mutex));
}

void	Socket::lockMutex(void)
{
	pthread_mutex_lock(&(this->mutex));
}

void	Socket::unlockMutex(void)
{
	pthread_mutex_unlock(&(this->mutex));
}

int	Socket::Read()
{
	int	n;

	n = this->read.recv(this->fd, 512);
	if (n == -1)
	{
		std::cerr << "Error recv" << std::endl;
		this->alive = false;
		return -1;
	}
	else if (n == 0)
		return -1;
	int	i;
	i = 0;
	while (i < n)
	{
//		std::cout << (int)this->read.buf[this->read.buf_pos + i] << " ";
		++i;
	}
//	std::cout << "\nn=" << n << "\n";
	return 0;
}

void	Socket::Write()
{
//	int	n;
//	char*	tmp;

	this->lockMutex();
	while (this->send.size() > 0)
		this->send.send(this->fd, this->send.size());

/*
	n = ::send(this->fd, this->send, strlen(this->send), 0);
	tmp = this->send + n;
	while (strlen(tmp) > 0)
	{
		n = ::send(this->fd, tmp, strlen(tmp), 0);
		tmp = this->send + n;
	}
	this->send[0] = 0;
*/	this->unlockMutex();
}
