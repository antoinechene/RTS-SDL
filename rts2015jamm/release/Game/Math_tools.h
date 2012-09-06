#ifndef	__MATH_TOOLS_H__
#define	__MATH_TOOLS_H__

double	IDsqrt(double n);
int	IDabs(int n);
double	IDdistance(double x1, double y1, double x2, double y2);

template <typename type>
inline type	pow(type n, int pow)
{
	type	ret;

	if (pow == 0)
		return 1;
	else if (pow == 1)
		return n;
	ret = n;
	while (pow > 1)
	{
		ret = ret * n;
		--pow;
	}
	return ret;
};

#endif
