#include "miniRT.h"

static void	set_plane_face_normal(t_hit_record *rec, t_ray *ray, t_plane *plane)
{
	int	front_face;

	front_face = dot(ray->direction, plane->normal) < 0;
	if (!front_face)
		rec->normal = vec3_times_d(plane->normal, -1.0);
	else
		rec->normal = plane->normal;
}

/* static void	get_plane_uv(t_plane *plane, t_hit_record *rec, double *u, double *v)
{
	(void)rec;
	t_vec3	point_on_plane;
	t_vec3	a;
	t_vec3	b;
	double	x;

	if (dot(plane->normal, init_vec3(0, 0, 1)) == 0)
		a = init_vec3(0, 0, 1);
	else
	{
		x = dot(plane->normal, plane->point);
		point_on_plane = init_vec3(0, 0, x);
		a = unit_vector(cross(plane->normal, vec3_plus_vec3(point_on_plane, plane->point)));
	}
	b = unit_vector(cross(plane->normal, a));

} */

static void	get_albedo(t_plane *plane, t_hit_record *rec)
{
	if (plane->texture_type == solid)
	{
		rec->albedo = plane->albedo;
		return ;
	}
	if (plane->texture_type == checker)
	{
		rec->albedo = get_checkered_color(rec->point,
				plane->checker_size_coeff, plane->albedo, plane->checker_color);
		return ;
	}
	if (plane->texture_type == texture)
	{
		rec->albedo = get_texture_color(plane->texture, rec->u, rec->v);
		return ;
	}
	if (plane->texture_type == bump_map)
	{
		rec->albedo = get_bump_map_color(plane->texture, rec->u, rec->v);
	}
}

int	hit_plane(t_ray *ray, t_interval t_minmax,
		t_hit_record *rec, t_plane *plane)
{
	double	denom;
	double	t;

	denom = dot(plane->normal, ray->direction);
	// printf("%f %f %f\n", plane->normal.x, plane->normal.y, plane->normal.z);
	if (fabs(denom) < 0.00000001)
		return (false);
	t = (dot(plane->normal, plane->point)
			- dot(plane->normal, ray->origin)) / denom;
	if (!contains(t_minmax, t))
		return (0);
	rec->point = ray_at(*ray, t);
	rec->material = plane->material;
	rec->material1 = plane->material1;
	set_plane_face_normal(rec, ray, plane);
	get_albedo(plane, rec);
	return (1);
}
