#include "miniRT.h"

t_interval	init_interval(double min, double max)
{
	t_interval	ret;

	ret.min = min;
	ret.max = max;
	return (ret);
}

int	contains(t_interval i, double x)
{
	return (x <= i.max && x >= i.min);
}

int	surrounds(t_interval i, double x)
{
	return (i.min < x && x < i.max);
}
