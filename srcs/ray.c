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

unsigned int	colorsum_to_rgba(t_color c, int samples_per_pixel)
{
	double	scale;

	scale = 1.0 / samples_per_pixel;
	c.x *= scale;
	c.y *= scale;
	c.z *= scale;
	return (lround(c.x * 255) << 24 | lround(c.y * 255) << 16
		| lround(c.z * 255) << 8 | 255);
}

int	hit(t_master *m, t_ray r, t_interval t_minmax, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	int				hit_anything;
	double			closest_so_far;
	unsigned int	i;

	i = 0;
	hit_anything = 0;
	closest_so_far = t_minmax.max;
	while (i < m->n_spheres)
	{
		if (hit_sphere(r, init_interval(t_minmax.min, closest_so_far),
				&temp_rec, m->spheres[i]))
		{
			hit_anything = 1;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		i++;
	}
	return (hit_anything);
}

t_color	ray_color(t_master *m, t_ray r)
{
	t_vec3			unit_direction;
	double			a;
	t_color			ret;
	t_hit_record	rec;

	if (hit(m, r, init_interval(0, INFINITY), &rec))
	{
		return (vec3_times_d(vec3_plus_vec3(rec.normal,
					init_vec3(1.0, 1.0, 1.0)), 0.5));
	}
	unit_direction = unit_vector(r.direction);
	a = 0.5 * (unit_direction.y + 1.0);
	ret = vec3_plus_vec3(vec3_times_d(init_vec3(1.0, 1.0, 1.0), 1.0 - a),
			vec3_times_d(init_vec3(0.5, 0.7, 1.0), a));
	return (ret);
}
