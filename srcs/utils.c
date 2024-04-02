#include "miniRT.h"

double	degrees_to_radians(double degrees)
{
	return (degrees * M_PI / 180);
}

double	random_double(void)
{
	return (rand() * 1.0 / (RAND_MAX + 1.0));
}

double	random_double_between(double min, double max)
{
	return (min + (max - min) * random_double());
}

double	linear_to_gamma(double linear)
{
	return (sqrt(linear));
}
