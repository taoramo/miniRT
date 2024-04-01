#include "miniRT.h"

t_vec3	ray_at(t_ray r, double t)
{
	return (vec3_plus_vec3(r.origin, vec3_times_d(r.direction, t)));
}

t_ray	init_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray	ret;

	ret.origin = origin;
	ret.direction = direction;
	return (ret);
}

unsigned int	color_to_rgba(t_color c)
{
	return (lround(c.x * 255) << 24 | lround(c.y * 255) << 16
		| lround(c.z * 255) << 8 | 255);
}

t_color	ray_color(t_master *m, t_ray r)
{
	t_vec3	unit_direction;
	double	a;
	t_color	ret;
	double	t;
	t_vec3	normal;

	t = hit_sphere(init_vec3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		normal = unit_vector(vec3_minus_vec3(ray_at(r, t),
					init_vec3(0, 0, -1)));
		return (vec3_times_d(init_vec3(normal.x
					+ 1, normal.y + 1, normal.z + 1), 0.5));
	}
	(void)m;
	unit_direction = unit_vector(r.direction);
	a = 0.5 * (unit_direction.y + 1.0);
	ret = vec3_plus_vec3(vec3_times_d(init_vec3(1.0, 1.0, 1.0), 1.0 - a),
			vec3_times_d(init_vec3(0.5, 0.7, 1.0), a));
	return (ret);
}
