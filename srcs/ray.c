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
	i = 0;
	while (i < m->n_planes)
	{
		if (hit_plane(r, init_interval(t_minmax.min, closest_so_far),
				&temp, &m->planes[i]))
		{
			hit_anything = 1;
			closest_so_far = temp.t;
			*rec = temp;
		}
		i++;
	}
	i = 0;
	while (i < m->n_cylinders)
	{
		if (hit_cylinder(r, init_interval(t_minmax.min, closest_so_far),
				&temp, &m->cylinders[i]))
		{
			hit_anything = 1;
			closest_so_far = temp.t;
			*rec = temp;
		}
		i++;
	}
	return (hit_anything);
}

t_vec3	ray_color(t_master *m, t_ray *r, int depth)
{
	t_hit_record	rec;
	t_ray			scattered_diffuse;
	t_ray			scattered_specular;
	t_vec3			attenuation_diffuse;
	t_vec3			attenuation_specular;
	t_vec3			emitted;

	attenuation_diffuse = init_vec3(0, 0, 0);
	attenuation_specular = init_vec3(0, 0, 0);
	emitted = init_vec3(0, 0, 0);
	ft_bzero(&rec, sizeof(rec));
	if (depth <= 0)
		return (init_vec3(0, 0, 0));
	if (!hit(m, r, init_interval(0.001, INFINITY), &rec))
		return (m->camera->background_color);
	if (lambertian_scatter(r, &rec, &scattered_diffuse))
	{
		attenuation_diffuse = vec3_times_d(rec.albedo, rec.k_d);
		emitted = rec.emitted;
	}
	if (metal_scatter(r, &rec, &scattered_specular))
	{
		attenuation_specular = vec3_times_d(rec.albedo, rec.k_s);
		emitted = vec3_plus_vec3(emitted, rec.emitted);
	}
	return (vec3_plus_vec3(emitted, vec3_plus_vec3(vec3_times_vec3(attenuation_specular, ray_color(m, &scattered_specular, depth - 1)), vec3_times_vec3(attenuation_diffuse, ray_color(m, &scattered_diffuse, depth - 1)))));
}
