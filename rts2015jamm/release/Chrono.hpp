#ifndef	__CHRONO_HPP__
#define	__CHRONO_HPP__

#include <sys/time.h>

class	Chrono
{
	public:
	Chrono()
	{
	}

	unsigned int	SaveTime(void)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return 0;
		this->last = tv.tv_sec * 1000000 + tv.tv_usec;
		return this->last;
	}

	unsigned int	CompareLast(void)
	{
		if (gettimeofday(&tv, NULL) == -1)
			return 0;
		return tv.tv_sec * 1000000 + tv.tv_usec - last;
	}

	unsigned int	NewTime(void)
	{
		unsigned int	ret;

		if (gettimeofday(&tv, NULL) == -1)
			return 0;
		ret = tv.tv_sec * 1000000 + tv.tv_usec - last;
		this->last = tv.tv_sec * 1000000 + tv.tv_usec;
		return ret;
	}

	struct timeval	tv;
	unsigned int	last;
};

#endif
