#include "Join_server.h"
#include "Sockets_manager.h"
#include <iostream>

Join_server::Join_server()
{
	this->ip = new char[16];
	this->ip[0] = 0;
}

Join_server::~Join_server()
{
	delete[] this->ip;
}

bool	Join_server::join()
{
	Sockets_manager*	sm;
	Socket*			opp;

	sm = Sockets_manager::GetInstance();
	try
	{
		opp = sm->connect_socket((char*)PORT, this->ip);
		if (opp != NULL)
		{
			if (!opp->start_com())
			{
				std::cout << "Thread work" << std::endl;
				return (true);
			}
			std::cout << "Error" << std::endl;
			delete opp;
		}
	}
	catch (int e)
	{
		if (e < 0)
		{
			std::cout << "Fatal error: " << e << std::endl;
		}
		std::cout << "error: " << e << " cannot connect" << std::endl;
	}
	std::cout << "connect failed, try another ip" << std::endl;
	return (false);
}

void	Join_server::change_ip(char* ip)
{
	this->ip = ip;
}
