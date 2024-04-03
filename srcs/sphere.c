#include "miniRT.h"

static void	get_sphere_uv(t_vec3 point, double *u, double *v)
{
	double	theta;
	double	phi;

	theta = acos(-1.0 * point.y);
	phi = atan2(-1.0 * point.z, point.x) + M_PI;
	*u = phi / (2 * M_PI);
	*v = theta / M_PI;
}

static void	set_sphere_face_normal(t_hit_record *rec,
			t_ray *r, t_sphere *sphere)
{
	t_vec3	outward_normal;

	outward_normal = vec3_div_d(vec3_minus_vec3(rec->point,
				sphere->origin), sphere->radius);
	get_sphere_uv(outward_normal, &rec->u, &rec->v);
	rec->front_face = dot(r->direction, outward_normal) < 0;
	if (rec->front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_times_d(outward_normal, -1.0);
}

static void	get_albedo(t_sphere *sphere, t_hit_record *rec)
{
	if (sphere->texture_type == solid)
	{
		rec->albedo = sphere->albedo;
		return ;
	}
	if (sphere->texture_type == checker)
	{
		rec->albedo = get_checkered_color(rec->point,
				sphere->checker_size_coeff,
				sphere->albedo, sphere->checker_color);
		return ;
	}
	if (sphere->texture_type == texture)
	{
		rec->albedo = get_texture_color(sphere->texture, rec->u, rec->v);
		return ;
	}
	if (sphere->texture_type == bump_map)
	{
		rec->albedo = get_bump_map_color(sphere->texture, rec->u, rec->v);
	}
}

int	hit_sphere(t_ray *ray, t_interval t_minmax,
			t_hit_record *rec, t_sphere *sphere)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	discriminant;
	double	root;

	oc = vec3_minus_vec3(ray->origin, sphere->origin);
	a = vec3length_squared(ray->direction);
	half_b = dot(oc, ray->direction);
	discriminant = half_b * half_b - a
		* (vec3length_squared(oc) - sphere->radius * sphere->radius);
	if (discriminant < 0)
		return (0);
	root = (-1.0 * half_b - sqrt(discriminant)) / a;
	if (!surrounds(t_minmax, root))
	{
		root = (-1.0 * half_b + sqrt(discriminant)) / a;
		if (!surrounds(t_minmax, root))
			return (0);
	}
	rec->t = root;
	rec->point = ray_at(*ray, root);
	rec->material = sphere->material;
	rec->material1 = sphere->material1;
	set_sphere_face_normal(rec, ray, sphere);
	get_albedo(sphere, rec);
	return (1);
}
