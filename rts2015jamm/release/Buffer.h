#ifndef	__BUFFER_H__
#define	__BUFFER_H__

#include <unistd.h>
/*
SCHEMA Buffer

 <---capacity-->
|_____------____|
 <pos>
      <size>
*/

class	Buffer
{
public:
	Buffer(size_t, bool is_ext = false);
	~Buffer();
	size_t	size() const;
	size_t	capacity() const;
	size_t	space() const;
	bool	empty() const;
	bool	full() const;

	size_t	check_size(size_t nbread);
	size_t	read(int fd, size_t nbread);
	size_t	recv(int fd, size_t nbread);
	size_t	send(int fd, size_t nbread, int flags = 0);
	size_t	read_send(int from, int to, size_t nbread);
	size_t	addstr(const char* str, size_t len);
	size_t	addstr(void* str, size_t len);
	size_t	getstr(void* str, size_t len);
	bool	getchar(char* ptr);
	char	getchar_nv();
	bool	peekchar(char* ptr, size_t index) const;
	char	peekchar_nv(size_t index) const;

	char*	getline();
	char*	peekline(size_t index) const;
	char*	flush(size_t new_capacity = 0);
	void	clear();

	Buffer*	clone();
	void	swap(Buffer* right);

	void	erase(size_t pos, size_t n);
	int	compare(const char* s) const;
	size_t	find(const char* str, size_t pos, size_t n) const;
	size_t	rfind(const char* str, size_t pos, size_t n) const;

public:
	bool	is_extensible;
	size_t	buf_size;
	size_t	buf_capacity;
	size_t	buf_pos;
	char*	buf;
};

#endif
