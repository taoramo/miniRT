#include "miniRT.h"

static void	set_plane_uv(t_hit_record *rec)
{
	rec->u_vector = unit_vector(cross(rec->normal, init_vec3(1, 0, 0)));
	if (!rec->u_vector.x && !rec->u_vector.y && !rec->u_vector.z)
		rec->u_vector = unit_vector(cross(rec->normal, init_vec3(0, 0, 1)));
	rec->v_vector = unit_vector(cross(rec->normal, rec->u_vector));
	rec->u = dot(rec->u_vector, rec->point)
		- floor(dot(rec->u_vector, rec->point));
	rec->v = dot(rec->v_vector, rec->point)
		- floor(dot(rec->v_vector, rec->point));
}

static void	set_plane_face_normal(t_hit_record *rec, t_ray *ray, t_plane *plane)
{
	int	front_face;

	front_face = dot(ray->direction, plane->normal) < 0;
	if (!front_face)
		rec->normal = vec3_times_d(plane->normal, -1.0);
	else
		rec->normal = plane->normal;
	set_plane_uv(rec);
	if (plane->texture.bump_map)
		rec->normal = bump_map(rec, plane->texture.bump_map);
}

static void	get_albedo(t_plane *plane, t_hit_record *rec)
{
	if (plane->texture.type == SOLID)
	{
		rec->albedo = plane->albedo;
		return ;
	}
	if (plane->texture.type == CHECKER)
	{
		rec->albedo = get_solid_checkered_color(rec->point,
				plane->texture.checker_size_coeff, plane->albedo, plane->texture.checker_color);
		return ;
	}
	if (plane->texture.type == PNG_FILE)
	{
		rec->albedo = get_texture_color(plane->texture.texture_obj, rec->u, rec->v);
		return ;
	}
}

void	set_plane_rec(t_hit_record *rec, t_plane *plane, t_ray *ray, double t)
{
	rec->point = ray_at(*ray, t);
	// rec->material = plane->material; // ?
	rec->material1 = plane->material1;
	rec->emitted = plane->emitted;
	rec->k_s = plane->k_s;
	rec->k_d = plane->k_d;
	set_plane_face_normal(rec, ray, plane);
	get_albedo(plane, rec);
}

int	hit_plane(t_ray *ray, t_interval t_minmax,
		double *t_new, t_plane *plane)
{
	double	denom;
	double	t;

	denom = dot(plane->normal, ray->direction);
	if (fabs(denom) < EPSILON)
		return (0);
	t = (dot(plane->normal, plane->point)
			- dot(plane->normal, ray->origin)) / denom;
	if (!contains(t_minmax, t))
		return (0);
	*t_new = t;
	return (1);
}
