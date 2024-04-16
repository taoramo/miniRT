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

t_vec3	phong_model(t_hit_record *rec, t_ray *shadow_ray,
			t_ray *ray, t_light *light)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	halfway_dir;
	t_vec3	specular;
	t_vec3	diffuse;

	light_dir = vec3_times_d(unit_vector(shadow_ray->direction), 1.0);
	view_dir = vec3_times_d(unit_vector(ray->direction), -1.0);
	halfway_dir = unit_vector(vec3_plus_vec3(light_dir, view_dir));
	specular = vec3_times_d(light->color,
			pow(dot(rec->normal, halfway_dir),
				fmax(10, 1.0 / rec->material1)) * rec->k_s);
	diffuse = vec3_times_vec3(vec3_times_d(light->color,
				dot(rec->normal, light_dir)
				* rec->k_d), rec->albedo);
	if (dot(rec->normal, light_dir) < 0 || dot(rec->normal, halfway_dir) < 0)
		specular = init_vec3(0, 0, 0);
	if (dot(rec->normal, light_dir) < 0)
		diffuse = init_vec3(0, 0, 0);
	return (vec3_plus_vec3(specular, diffuse));
}

t_vec3	shadow_ray(t_master *m, t_hit_record *rec, t_ray *ray)
{
	unsigned int		i;
	t_ray				shadow_ray;
	t_vec3				return_color;
	double				length;

	return_color = init_vec3(0, 0, 0);
	i = 0;
	while (i < m->n_lights)
	{
		shadow_ray.origin = rec->point;
		shadow_ray.direction = vec3_times_d(vec3_minus_vec3(m->lights[i].point, rec->point), 1.0);
		length = vec3length(shadow_ray.direction);
		shadow_ray.direction = unit_vector(shadow_ray.direction);
		if (!hit(m, &shadow_ray, init_interval(0.001, length), 0))
			return_color = vec3_plus_vec3(return_color,
					phong_model(rec, &shadow_ray, ray, &m->lights[i]));
		i++;
	}
	return (return_color);
}

t_vec3	ray_color(t_master *m, t_ray *r, int depth)
{
	t_hit_record	rec;
	t_ray			scattered_diffuse;
	t_ray			scattered_specular;
	t_ray_colors	colors;

	ft_bzero(&rec, sizeof(rec));
	ft_bzero(&colors, sizeof(colors));
	if (depth <= 0)
		return (init_vec3(0, 0, 0));
	if (!hit(m, r, init_interval(0.001, INFINITY), &rec))
		return (m->camera.background_color);
	if (depth == m->max_depth && m->n_lights > 0 && rec.emitted.x == 0
		&& rec.emitted.y == 0 && rec.emitted.z == 0)
		colors.shadow = shadow_ray(m, &rec, r);
	if (rec.k_d && lambertian_scatter(r, &rec, &scattered_diffuse))
		colors.diffuse = vec3_times_d(rec.albedo, rec.k_d);
	if (rec.k_s && metal_scatter(r, &rec, &scattered_specular))
		colors.specular = vec3_times_d(rec.albedo, rec.k_s);
	colors.emitted = rec.emitted;
	return (vec3_plus_vec3(colors.shadow, vec3_plus_vec3(colors.emitted,
				vec3_plus_vec3(vec3_times_vec3(colors.specular,
						ray_color(m, &scattered_specular, depth - 1)),
					vec3_times_vec3(colors.diffuse,
						ray_color(m, &scattered_diffuse, depth - 1))))));
}
