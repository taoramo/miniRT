#include "miniRT.h"

double	dot(t_vec3 u, t_vec3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

t_vec3	random_vec3(void)
{
	return (init_vec3(random_double(), random_double(), random_double()));
}

t_vec3	random_vec3_between(double min, double max)
{
	return (init_vec3(random_double_between(min, max),
			random_double_between(min, max),
			random_double_between(min, max)));
}

t_vec3	random_in_unit_sphere(void)
{
	t_vec3	p;

	while (1)
	{
		p = random_vec3_between(-1, 1);
		if (vec3length_squared(p) < 1)
			return (p);
	}
}

t_vec3	random_unit_vector(void)
{
	return (unit_vector(random_in_unit_sphere()));
}
