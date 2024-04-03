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
