#include "miniRT.h"

double	hit_sphere(t_vec3 center, double radius, t_ray ray)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;

	oc = vec3_minus_vec3(ray.origin, center);
	a = vec3length_squared(ray.direction);
	half_b = dot(oc, ray.direction);
	c = vec3length_squared(oc) - radius * radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (-1.0);
	else
		return (-half_b - sqrt(discriminant) / a);
}
