#ifndef	__SOCKETS_MANAGER_H__
#define	__SOCKETS_MANAGER_H__

#include "../ID_glibrary.h"
#include "../Singleton.hpp"
#include <vector>

class	Socket;
class	Sockets_manager : public Singleton<Sockets_manager>
{
	friend class	Singleton<Sockets_manager>;
 public:
	Socket*			ListenSocket(char* port);
	Socket*			ConnectSocket(char* port, char* ip);
	int			AcceptFromHost(void);
	int			CloseSockets();
	std::vector<Socket*>*	GetSockets();
	void			SetHostSocket(Socket* host);
	Socket*			GetHostSocket(void);
	Socket*			GetListenSocket(void);
	static void*		Accept(void* fd);
	void			ClearSockets();
	void			DeleteSocket(Socket* s);
	void			DeleteSocket(std::vector<Socket*>::iterator it);
 private:
		Sockets_manager();
		~Sockets_manager();
	int	__ListenSocket(char* port);
	Socket*	__GetClientSocket(char* port, char* ip);
 private:
	bool			alive;
	Socket*			hostSocket;
	Socket*			listenSocket;
	std::vector<Socket*>	clientSockets;
};

#endif
