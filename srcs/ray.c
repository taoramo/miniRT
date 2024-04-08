#include "miniRT.h"
#include "vec3.h"

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
			if (rec)
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
			if (rec)
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
			if (rec)
				*rec = temp;
		}
		i++;
	}
	return (hit_anything);
}

t_vec3	phong_model(t_hit_record *rec, t_ray *shadow_ray, t_ray *ray, t_light *light)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	halfway_dir;
	t_vec3	specular;
	t_vec3	diffuse;

	light_dir = vec3_times_d(unit_vector(shadow_ray->direction), -1.0);
	view_dir = unit_vector(ray->direction);
	halfway_dir = vec3_plus_vec3(light_dir, view_dir);
	specular = vec3_times_d(light->color, pow(fmax(dot(rec->normal, halfway_dir), 0.0), 1.0 / rec->material1) * rec->k_s);
	diffuse = vec3_times_vec3(vec3_times_d(light->color, fmax(0.f, dot(rec->normal, light_dir)) * rec->k_d), rec->albedo);
	// printf("R %f G %f B %f\n", shadow_ray->direction.x, shadow_ray->direction.y, shadow_ray->direction.z);
	// printf("%f\n", dot(rec->normal, light_dir));
	return (vec3_plus_vec3(specular, diffuse));
}

t_vec3	shadow_ray(t_master *m, t_hit_record *rec, t_ray *ray)
{
	unsigned int		i;
	t_ray				shadow_ray;
	t_vec3				return_color;

	return_color = init_vec3(0, 0, 0);
	i = 0;
	while (i < m->n_lights)
	{
		shadow_ray.origin = rec->point;
		shadow_ray.direction = vec3_minus_vec3(m->lights[0].point, rec->point);
		if (!hit(m, &shadow_ray, init_interval(0.001, INFINITY), 0))
			return_color = vec3_plus_vec3(return_color, phong_model(rec, ray, ray, &m->lights[i]));
		i++;
	}
	return (return_color);
}

t_vec3	ray_color(t_master *m, t_ray *r, int depth)
{
	t_hit_record	rec;
	t_ray			scattered_diffuse;
	t_ray			scattered_specular;
	t_vec3			attenuation_diffuse;
	t_vec3			attenuation_specular;
	t_vec3			color_shadow_ray;
	t_vec3			emitted;

	attenuation_diffuse = init_vec3(0, 0, 0);
	attenuation_specular = init_vec3(0, 0, 0);
	emitted = init_vec3(0, 0, 0);
	color_shadow_ray = init_vec3(0, 0, 0);
	ft_bzero(&rec, sizeof(rec));
	if (depth <= 0)
		return (init_vec3(0, 0, 0));
	if (!hit(m, r, init_interval(0.001, INFINITY), &rec))
		return (m->camera.background_color);
	if (depth == m->max_depth && m->n_lights > 0 && rec.emitted.x == 0 && rec.emitted.y == 0 && rec.emitted.z == 0)
		color_shadow_ray = shadow_ray(m, &rec, r);
//	printf("R %f G %f B %f\n", color_shadow_ray.x, color_shadow_ray.y, color_shadow_ray.z);
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
	return (vec3_plus_vec3(color_shadow_ray, vec3_plus_vec3(emitted, vec3_plus_vec3(vec3_times_vec3(attenuation_specular, ray_color(m, &scattered_specular, depth - 1)), vec3_times_vec3(attenuation_diffuse, ray_color(m, &scattered_diffuse, depth - 1))))));
}
