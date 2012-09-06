#ifndef __JOIN_SERVER_H__
#define __JOIN_SERVER_H__

#define PORT	"3343"

class	Join_server
{
 public:
	Join_server(void);
	~Join_server(void);

	bool	join(void);
	void	change_ip(char* ip);
 private:
	char*	ip;
//host socket
//join socket

};

#endif
