#include "miniRT.h"
#include "vec3.h"

static void	set_cylinder_uv(t_hit_record *rec, t_cylinder *cylinder, t_ray *ray)
{
	double	m;
	t_vec3	oc;

	rec->u = 1 - atan2(rec->point.x, rec->point.z) / (2 * M_PI) + 0.5;
	rec->v = rec->point.y - floor(rec->point.y);
	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center, vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	m = dot(ray->direction, cylinder->axisnormal) * rec->t + dot(oc, cylinder->axisnormal);
	rec->v = m / cylinder->height;
}

static void	set_cylinder_face_normal(t_hit_record *rec, t_ray *ray, t_cylinder *cylinder)
{
	t_vec3	outward_normal;
	double	m;
	t_vec3	oc;

	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center, vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	m = dot(ray->direction, cylinder->axisnormal) * rec->t + dot(oc, cylinder->axisnormal);
	outward_normal = vec3_minus_vec3(rec->point, vec3_minus_vec3(cylinder->center, vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	outward_normal = unit_vector(vec3_minus_vec3(outward_normal, vec3_times_d(cylinder->axisnormal, m)));
	rec->normal = outward_normal;
	set_cylinder_uv(rec, cylinder, ray);
}

static void	get_albedo(t_cylinder *cylinder, t_hit_record *rec)
{
	if (cylinder->texture_type == solid)
	{
		rec->albedo = cylinder->albedo;
		return ;
	}
	if (cylinder->texture_type == checker)
	{
		rec->albedo = get_checkered_color(rec->point,
				cylinder->checker_size_coeff,
				cylinder->albedo, cylinder->checker_color);
		return ;
	}
	if (cylinder->texture_type == texture)
	{
		rec->albedo = get_texture_color(cylinder->texture, rec->u, rec->v);
		return ;
	}
	if (cylinder->texture_type == bump_map)
	{
		rec->albedo = get_bump_map_color(cylinder->texture, rec->u, rec->v);
	}
}

static int	check_root(double root, t_ray *ray, t_cylinder *cylinder)
{
	double	m;
	t_vec3	oc;

	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center, vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	m = dot(ray->direction, cylinder->axisnormal) * root + dot(oc, cylinder->axisnormal);
	if (surrounds(init_interval(0, cylinder->height), m))
		return (1);
	else
		return (0);
}

int	hit_cylinder(t_ray *ray, t_interval t_minmax, t_hit_record *rec, t_cylinder *cylinder)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;
	double	root1;
	double	root2;
	int		root1_is_valid;
	int		root2_is_valid;

	root1_is_valid = 0;
	root2_is_valid = 0;
	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center, vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	a = vec3length_squared(ray->direction) - dot(ray->direction, cylinder->axisnormal) * dot(ray->direction, cylinder->axisnormal);
	if (fabs(a) < 0.00000001)
		return (0);
	half_b = dot(oc, ray->direction) - dot(ray->direction, cylinder->axisnormal) * dot(oc, cylinder->axisnormal);
	c = vec3length_squared(oc) - dot(oc, cylinder->axisnormal) * dot(oc, cylinder->axisnormal) - cylinder->radius * cylinder->radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (0);
	root1 = (-1 * half_b - sqrt(discriminant)) / a;
	root2 = (-1.0 * half_b + sqrt(discriminant)) / a;
	if (surrounds(t_minmax, root1) && check_root(root1, ray, cylinder))
		root1_is_valid = 1;
	if (surrounds(t_minmax, root2) && check_root(root2, ray, cylinder))
		root2_is_valid = 1;
	if (root1_is_valid && root2_is_valid)
	{
		if (root1 < root2)
			rec->t = root1;
		else
			rec->t = root2;
	}
	else if (root1_is_valid)
		rec->t = root1;
	else if (root2_is_valid)
		rec->t = root2;
	else
		return (0);
	rec->point = ray_at(*ray, rec->t);
	rec->material = cylinder->material;
	rec->material1 = cylinder->material1;
	set_cylinder_face_normal(rec, ray, cylinder);
	get_albedo(cylinder, rec);
	return (1);
}
