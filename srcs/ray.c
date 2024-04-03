#include "miniRT.h"

unsigned int	colorsum_to_rgba(t_color c, int samples_per_pixel)
{
	double		scale;
	t_interval	i;

	scale = 1.0 / samples_per_pixel;
	c.x *= scale;
	c.y *= scale;
	c.z *= scale;
	c.x = linear_to_gamma(c.x);
	c.y = linear_to_gamma(c.y);
	c.z = linear_to_gamma(c.z);
	i = init_interval(0.000, 0.999);
	return (lround(clamp(i, c.x) * 255) << 24
		| lround(clamp(i, c.y) * 255) << 16
		| lround(clamp(i, c.z) * 255) << 8 | 255);
}

int	hit(t_master *m, t_ray *r, t_interval t_minmax, t_hit_record *rec)
{
	int				hit_anything;
	double			closest_so_far;
	unsigned int	i;
	t_hit_record	temp;

	i = 0;
	hit_anything = 0;
	closest_so_far = t_minmax.max;
	while (i < m->n_spheres)
	{
		if (hit_sphere(r, init_interval(t_minmax.min, closest_so_far),
				&temp, &m->spheres[i]))
		{
			hit_anything = 1;
			closest_so_far = temp.t;
			*rec = temp;
		}
		i++;
	}
	return (hit_anything);
}

int	run_scatter(t_ray *r, t_hit_record *rec,
			t_ray *scattered, t_vec3 *attenuation)
{
	const t_f		scatter_functions[3] = {lambertian_scatter,
		metal_scatter, matte_scatter};

	*attenuation = rec->albedo;
	return (scatter_functions[rec->material](r, rec, scattered));
}

t_color	ray_color(t_master *m, t_ray *r, int depth)
{
	t_vec3			unit_direction;
	t_color			ret;
	t_hit_record	rec;
	t_ray			scattered;
	t_vec3			attenuation;

	if (depth <= 0)
		return (init_vec3(0, 0, 0));
	if (hit(m, r, init_interval(0.001, INFINITY), &rec))
	{
		//printf("%f %f %f\n", rec.albedo.x, rec.albedo.y, rec.albedo.z);
		if (run_scatter(r, &rec, &scattered, &attenuation))
			return (vec3_times_vec3(attenuation,
					ray_color(m, &scattered, depth - 1)));
		return (init_vec3(0, 0, 0));
	}
	unit_direction = unit_vector(r->direction);
	ret = vec3_plus_vec3(vec3_times_d(init_vec3(1.0, 1.0, 1.0),
				1.0 - (0.5 * (unit_direction.y + 1.0))),
			vec3_times_d(init_vec3(0.5, 0.7, 1.0),
				0.5 * (unit_direction.y + 1.0)));
	return (ret);
}
