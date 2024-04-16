#include "miniRT.h"

t_vec3	get_cylinder_checkered_color(t_hit_record *rec, t_cylinder *cylinder)
{
	double	checker_u;
	double	checker_v;
	int		x;
	int		y;

	checker_u = rec->u * cylinder->texture.checker_size_coeff * 2;
	checker_v = rec->v * cylinder->texture.checker_size_coeff;
	x = floor(checker_u);
	y = floor(checker_v);
	if ((x + y) % 2)
		return (cylinder->albedo);
	else
		return (cylinder->texture.checker_color);
}

static void	set_cylinder_uv(t_hit_record *rec, t_cylinder *cylinder)
{
	rec->u = (atan2(-1.0 * rec->point.z, rec->point.x) + M_PI) / (2 * M_PI);
	rec->v = cylinder->m / cylinder->height;
}

static void	set_cylinder_face_normal(t_hit_record *rec,
				t_ray *ray, t_cylinder *cylinder)
{
	t_vec3	outward_normal;

	cylinder->oc = vec3_minus_vec3(ray->origin,
			vec3_minus_vec3(cylinder->center,
				vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	cylinder->m = dot(vec3_plus_vec3(rec->point, cylinder->oc),
			cylinder->axisnormal);
	outward_normal = vec3_minus_vec3(rec->point,
			vec3_minus_vec3(cylinder->center,
				vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	outward_normal = unit_vector(vec3_minus_vec3(outward_normal,
				vec3_times_d(cylinder->axisnormal, cylinder->m)));
	rec->normal = outward_normal;
	set_cylinder_uv(rec, cylinder);
	rec->v_vector = cylinder->axisnormal;
	rec->u_vector = cross(cylinder->axisnormal, outward_normal);
	if (cylinder->texture.bump_map)
		rec->normal = bump_map(rec, cylinder->texture.bump_map);
}

static void	get_albedo(t_cylinder *cylinder, t_hit_record *rec)
{
	if (cylinder->texture.type == SOLID)
	{
		rec->albedo = cylinder->albedo;
		return ;
	}
	if (cylinder->texture.type == CHECKER)
	{
		rec->albedo = get_cylinder_checkered_color(rec, cylinder);
		return ;
	}
	if (cylinder->texture.type == PNG_FILE)
	{
		rec->albedo = get_texture_color(cylinder->texture.texture_obj, rec->u, rec->v);
		return ;
	}
}

void	set_cylinder_rec(t_hit_record *rec,
			t_cylinder *cylinder, t_ray *ray, double t)
{
	rec->point = ray_at(*ray, t);
	// rec->material = cylinder->material; // ?
	rec->material1 = cylinder->material1;
	rec->emitted = cylinder->emitted;
	rec->k_s = cylinder->k_s;
	rec->k_d = cylinder->k_d;
	set_cylinder_face_normal(rec, ray, cylinder);
	get_albedo(cylinder, rec);
}
