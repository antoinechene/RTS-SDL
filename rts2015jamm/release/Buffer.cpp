#include "Buffer.h"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>

Buffer::Buffer(size_t size, bool is_ext) : is_extensible(is_ext), buf_size(0), buf_capacity(size), buf_pos(0)
{
//	std::cout << "making Buffer of size " << size << std::endl;
	try
	{
		this->buf = new char[size];
		if (this->buf == 0)
			perror("Buffer");
	}
	catch (...)
	{
		std::cout << "Buffer error" << std::endl;
	}
}

Buffer::~Buffer()
{
	delete this->buf;
}

size_t	Buffer::size() const
{
	return this->buf_size;
}

size_t	Buffer::capacity() const
{
	return this->buf_capacity;
}

size_t	Buffer::space() const
{
	return this->buf_capacity - this->buf_size;
}

bool	Buffer::empty() const
{
	if (this->buf_size == 0)
		return true;
	return false;
}

bool	Buffer::full() const
{
	if (this->buf_size == this->buf_capacity)
		return true;
	return false;
}

size_t	Buffer::check_size(size_t nbread)
{
	char*	tmp;
	size_t	_pos;
	size_t	_size;

	if (this->buf_capacity - this->buf_pos - this->buf_size < nbread)
	{
		if (this->is_extensible == true && this->space() < nbread)
		{
			_pos = this->buf_pos;
			_size = this->buf_size;
			if (nbread > this->buf_capacity + this->buf_capacity)
				this->buf_capacity = nbread;
			tmp = this->flush(this->buf_capacity + this->buf_capacity);
			if (tmp == NULL)
				return 0;
			strncpy(this->buf, tmp + _pos, _size);
			delete tmp;
			this->buf_size = _size;
			return nbread;
		}
		if (this->buf_pos != 0)
		{
			memcpy(this->buf, this->buf + this->buf_pos, this->buf_size);
			this->buf_pos = 0;
			if (this->buf_capacity - this->buf_size < nbread)
				nbread = this->buf_capacity - this->buf_size;
		}
		else
			nbread = this->buf_capacity - this->buf_size;
	}
	return (nbread);
}

size_t	Buffer::read(int fd, size_t nbread)
{
	size_t	nb;

	nbread = this->check_size(nbread);
	if (nbread > 0)
	{
//		std::cout << "fd" << fd << std::endl;
//		std::cout << "nbread" << nbread << std::endl;
//		std::cout << "buf_pos" << this->buf_pos << std::endl;
//		std::cout << "buf_size" << this->buf_size << std::endl;
		nb = ::read(fd, this->buf + this->buf_size + this->buf_pos, nbread);
		if (nb > 0)
			this->buf_size = this->buf_size + nb;
		return (nb);
	}
	return (0);
}

size_t	Buffer::recv(int fd, size_t nbread)
{
	size_t	nb;

	nbread = this->check_size(nbread);
	if (nbread > 0)
	{
		nb = ::recv(fd, this->buf + this->buf_size + this->buf_pos, nbread, 0);
		if (nb > 0)
			this->buf_size = this->buf_size + nb;
		return (nb);
	}
	return (0);
}

size_t	Buffer::send(int fd, size_t nbread, int flags)
{
	size_t	nb;

	if ((int)nbread > 0)
	{
		nb = ::send(fd, this->buf + this->buf_pos, nbread, flags);
		if (nb > 0)
		{
			this->buf_pos = this->buf_pos + nb;
			this->buf_size = this->buf_size - nb;
		}
		return (nb);
	}
	return (0);
}

size_t	Buffer::read_send(int from, int to, size_t nbread)
{
	size_t	n;
	size_t	nsend;

	n = this->read(from, nbread);
//	std::cout << "BUF=" << this->buf + this->buf_pos << std::endl;
	if ((signed int)n != -1)
		nsend = this->send(to, this->size());
	else
		return (0);
	std::cout << "Read " << n << "\nSend " << nsend << "(" << nbread << ") chars to " << to << std::endl;
	return nsend;
}

size_t	Buffer::addstr(const char* str, size_t len)
{
	size_t	i;

//	if (*str == 0)
//		return 0;
	len = check_size(len);
	if (len <= 0)
		return 0;
	i = 0;
	while (i < len)
	{
		this->buf[i + this->buf_pos + this->buf_size] = *str;
		str = str + 1;
		i = i + 1;
	}
	this->buf_size = this->buf_size + i;
	return i;
}

size_t	Buffer::addstr(void* _str, size_t len)
{
	size_t	i;
	char*	str;

	str = (char*)_str;
//	if (*str == 0)
//		return 0;
	len = check_size(len);
	if (len <= 0)
		return 0;
	i = 0;
	while (i < len)
	{
		this->buf[i + this->buf_pos + this->buf_size] = *str;
		str = str + 1;
		i = i + 1;
	}
	this->buf_size = this->buf_size + i;
	return i;
}

size_t	Buffer::getstr(void* _str, size_t len)
{
	size_t	i;
	char*	str;

	str = (char*)_str;
	i = 0;
	while (i < len && i < this->buf_size)
	{
		str[i] = this->buf[i + this->buf_pos];
		++i;
	}
	this->buf_pos += i;
	this->buf_size = this->buf_size - i;
	return i;
}

bool	Buffer::getchar(char* ptr)
{
	if (this->buf_size > 0)
	{
		*ptr = this->buf[this->buf_pos];
		this->buf[this->buf_pos] = 0;
		this->buf_size = this->buf_size - 1;
		this->buf_pos = this->buf_pos + 1;
		if (this->buf_size == 0)
			this->buf_pos = 0;
		return false;
	}
	return true;
}

char	Buffer::getchar_nv()
{
	char	ret;

	if (this->buf_size > 0)
	{
		ret = this->buf[this->buf_pos];
		this->buf[this->buf_pos] = 0;
		this->buf_size = this->buf_size - 1;
		this->buf_pos = this->buf_pos + 1;
		if (this->buf_size == 0)
			this->buf_pos = 0;
		return ret;
	}
	return 0;
}

bool	Buffer::peekchar(char* ptr, size_t index) const
{
	if (this->buf_size > 0)
	{
		*ptr = this->buf[this->buf_pos + index];
		return false;
	}
	*ptr = 0;
	return true;
}

char	Buffer::peekchar_nv(size_t index) const
{
	if (this->buf_size > 0)
		return this->buf[this->buf_pos + index];
	return 0;
}

char*	Buffer::getline()
{
	char*	str;
	size_t	i;
	size_t	l;

	l = 0;
	while (l < this->buf_size &&
	       this->buf[l + this->buf_pos] != '\n')
		l = l + 1;
	if (l + this->buf_pos >= this->buf_capacity ||
	    this->buf[l + this->buf_pos] != '\n')
		return 0;
	l = l + 1;
	str = new char[l + 1];
	if (str == 0)
		return (0);
	i = 0;
	while (i < l)
	{
		str[i] = this->buf[this->buf_pos + i];
		this->buf[this->buf_pos + i] = 0;
		i = i + 1;
	}
	str[i - 1] = 0;
	this->buf_pos = this->buf_pos + i;
	this->buf_size = this->buf_size - i;
	return str;
}

char*	Buffer::peekline(size_t index) const
{
	char*	str;
	size_t	i;
	size_t	l;

	l = 0;
	while (index + l + this->buf_pos < this->buf_size &&
	       this->buf[this->buf_pos + index + l] != '\n')
		l = l + 1;
	if (this->buf[this->buf_pos + index + l] != '\n' || l == 0)
		return (0);
	str = new char[l + 1];
	if (str == 0)
		return (0);
	i = 0;
	while (i < l)
	{
		str[i] = this->buf[this->buf_pos + index + i];
		i = i + 1;
	}
	str[i] = 0;
	return str;
}

char*	Buffer::flush(size_t new_capacity)
{
	char*	oldbuf;

	if (new_capacity == 0)
		new_capacity = this->buf_capacity;
	oldbuf = this->buf;
	this->buf = new char[new_capacity];
	if (this->buf == 0)
		return 0;
	this->buf_pos = 0;
	this->buf_size = 0;
	this->buf_capacity = new_capacity;
	return oldbuf;
}

void	Buffer::clear()
{
/*	size_t	i;

	i = 0;
	while (i < this->buf_capacity)
	{
		this->buf[i] = 0;
		i = i + 1;
	}
*/	this->buf_pos = 0;
	this->buf_size = 0;
}

Buffer*	Buffer::clone()
{
	Buffer*	buf;
	size_t	i;

	buf = new Buffer::Buffer(this->buf_capacity);
	if (buf == 0)
		return 0;
	buf->buf_size = this->buf_size;
	buf->buf_pos = this->buf_pos;
	buf->buf_capacity = this->buf_capacity;
	i = 0;
	while (i < this->buf_capacity)
	{
		buf->buf[i] = this->buf[i];
		i = i + 1;
	}
	return buf;
}

void	Buffer::swap(Buffer* right)
{
	size_t	tmp_size;
	size_t	tmp_pos;
	size_t	tmp_capacity;
	char*	tmp_buf;

	tmp_size = this->buf_size;
	tmp_pos = this->buf_pos;
	tmp_capacity = this->buf_capacity;
	tmp_buf = this->buf;

	this->buf_size = right->buf_size;
	this->buf_pos = right->buf_pos;
	this->buf_capacity = right->buf_capacity;
	this->buf = right->buf;

	right->buf_size = tmp_size;
	right->buf_pos = tmp_pos;
	right->buf_capacity = tmp_capacity;
	right->buf = tmp_buf;
}

void	Buffer::erase(size_t pos, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	max;

	i = this->buf_pos + pos + n - 1;
	max = this->buf_size - n - pos;
	if (max <= 0 || i >= this->buf_capacity)
	{
		this->buf_size = pos;
		return ;
	}
	j = 0;
	while (j < max)
	{
		j = j + 1;
		i = i + 1;
		this->buf[i - n] = this->buf[i];
	}
	this->buf_size = this->buf_size - n;
}

int	Buffer::compare(const char* s) const
{
	char*	buf_tmp;
	size_t	i;

	i = this->buf_pos;
	buf_tmp = this->buf + this->buf_pos;
	while (*buf_tmp != 0 && *s != 0 && *buf_tmp == *s)
	{
		buf_tmp = buf_tmp + 1;
		s = s + 1;
		i = i + 1;
		if (i == this->buf_capacity && *s == 0)
			return 0;
	}
	return *buf_tmp - *s;
}

size_t	Buffer::find(const char* str, size_t pos, size_t n) const
{
	size_t	i;

	if (*str == 0)
		return pos;
	i = 0;
	while (pos < this->buf_size)
	{
		i = 0;
		while (this->buf[this->buf_pos + pos] == str[i] && i < n)
		{
			pos = pos + 1;
			i = i + 1;
			if (i == n)
				return pos - i;
		}
		pos = pos + 1;
	}
	return -1;
}

size_t	Buffer::rfind(const char* str, size_t pos, size_t n) const
{
	size_t	i;
	size_t	j;
	size_t	last_pos;
	size_t	break_pos;

	if (*str == 0)
		return 0;
	i = 0;
	j = 0;
	last_pos = -1;
	break_pos = 1;
	while (i <= pos)
	{
		while (this->buf[this->buf_pos + i] == str[j] &&
		       j < n && i <= pos)
		{
			i = i + 1;
			j = j + 1;
			if (j == n)
			{
				last_pos = i - j;
				i = break_pos + 1;
				break;
			}
		}
		j = 0;
		i = i + 1;
		break_pos = i;
	}
	return last_pos;
}
