#include "Math_tools.h"
#include "../Constantes.h"
#include <math.h>

double	IDsqrt(double n)
{
	long		i;
	double		x2;
	double		y;
	const double	threeehalfs = 1.5F;

	x2 = n * 0.5F;
	y = n;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(double*)&i;
	y = y * (threeehalfs - (x2 * y * y));
	return 1 / y;
}

int	IDabs(int n)
{
	return ((n >= 0) ? n : -n);
}

double	IDdistance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow<double>(x1 - x2, 2) + pow<double>(y1 - y2, 2));
}

