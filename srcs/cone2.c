/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:41:02 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/11 18:07:00 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	get_cone_checkered_color(t_hit_record *rec, t_cone *cone)
{
	double	checker_u;
	double	checker_v;
	int		x;
	int		y;
	int		cone_coeff;

	cone_coeff = 2; // just to play with the checker size to look same as for a sphere
	checker_u = rec->u * cone->checker_size_coeff * 4 / cone_coeff;
	checker_v = rec->v * cone->checker_size_coeff / cone_coeff;
	x = floor(checker_u);
	y = floor(checker_v);
	if ((x + y) % 2)
		return (cone->albedo);
	else
		return (cone->checker_color);
}

/* static void	set_cone_uv(t_hit_record *rec, t_cone *cone, t_ray *ray)
{
	
} */

static void	set_cone_uv(t_hit_record *rec, t_cone *cone, t_ray *ray)
{
	// double	m;
	// t_vec3	oc;
	(void)ray;
	rec->u = (1 - atan2(rec->point.x, rec->point.z) / (2 * M_PI) + 0.5);
/* 	oc = vec3_minus_vec3(ray->origin,
			vec3_minus_vec3(cone->tip,
				vec3_times_d(cone->axis, cone->height / 2)));
	m = dot(ray->direction, cone->axis)
		* rec->t + dot(oc, cone->axis);
	rec->v = m / cone->height; */
	rec->v = vec3length(vec3_minus_vec3(rec->point, cone->tip)) / (cone->height / 2);
}

static void	set_cone_face_normal(t_hit_record *rec,
				t_ray *ray, t_cone *cone)
{
	t_vec3	outward_normal;
	double	m;
	t_vec3	oc;

	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cone->tip,
				vec3_times_d(cone->axis, cone->height / 2)));
	m = dot(ray->direction, cone->axis)
		* rec->t + dot(oc, cone->axis);
	outward_normal = vec3_minus_vec3(rec->point,
			vec3_minus_vec3(cone->tip,
				vec3_times_d(cone->axis, cone->height / 2)));
	outward_normal = unit_vector(vec3_minus_vec3(outward_normal,
				vec3_times_d(cone->axis, m)));
	rec->normal = outward_normal;
	set_cone_uv(rec, cone, ray);
	rec->v_vector = cone->axis;
	rec->u_vector = cross(cone->axis, outward_normal);
	if (cone->bump_map)
		rec->normal = bump_map(rec, cone->bump_map);
}

static void	get_albedo(t_cone *cone, t_hit_record *rec)
{
	if (cone->texture_type == solid)
	{
		rec->albedo = cone->albedo;
		return ;
	}
	if (cone->texture_type == checker)
	{
		rec->albedo = get_cone_checkered_color(rec, cone);
		return ;
	}
	if (cone->texture_type == texture)
	{
		rec->albedo = get_texture_color(cone->texture, rec->u, rec->v);
		return ;
	}
}

void	set_cone_rec(t_hit_record *rec,
			t_cone *cone, t_ray *ray, double t)
{
	rec->point = ray_at(*ray, t);
	rec->material = cone->material;
	rec->material1 = cone->material1;
	rec->emitted = cone->emitted;
	rec->k_s = cone->k_s;
	rec->k_d = cone->k_d;
	set_cone_face_normal(rec, ray, cone);
	get_albedo(cone, rec);

	// cone->albedo = init_vec3(255,0,0);
	// rec->albedo = init_vec3(255,0,0);
}
