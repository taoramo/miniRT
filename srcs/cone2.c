/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:41:02 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 21:05:11 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * cone_coeff = 2 to have same checker size as for a sphere
*/
t_vec3	get_cone_checkered_color(t_hit_record *rec, t_cone *cone)
{
	double	checker_u;
	double	checker_v;
	int		x;
	int		y;
	int		cone_coeff;

	cone_coeff = 2;
	checker_u = rec->u * cone->texture.checker_size_coeff * 4 / cone_coeff;
	checker_v = rec->v * cone->texture.checker_size_coeff / cone_coeff;
	x = floor(checker_u);
	y = floor(checker_v);
	if ((x + y) % 2)
		return (cone->albedo);
	else
		return (cone->texture.checker_color);
}

static void	set_cone_uv(t_hit_record *rec, t_cone *cone)
{
	rec->u = (1 - atan2(rec->point.x, rec->point.z) / (2 * M_PI) + 0.5);
	if (rec->u > 1)
		rec->u = rec->u - floor(rec->u);
	rec->v = vec3length(vec3_minus_vec3(rec->point, cone->tip))
		/ (cone->height / 2);
	rec->u = rec->u - floor(rec->u);
	rec->v = rec->v - floor(rec->v);
}

static void	set_cone_face_normal(t_hit_record *rec, t_ray *r, t_cone *cone)
{
	t_vec3	outward_normal;
	t_vec3	tip_to_intersection;
	t_vec3	axis;
	int		front_face;

	tip_to_intersection = vec3_minus_vec3(cone->tip, rec->point);
	axis = cone->axis;
	if (rec->point.y > cone->tip.y)
		axis = vec3_times_d(axis, -1);
	outward_normal = cross(axis, tip_to_intersection);
	outward_normal = cross(tip_to_intersection, outward_normal);
	outward_normal = unit_vector(outward_normal);
	front_face = dot(r->direction, outward_normal) < 0;
	if (front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_times_d(outward_normal, -1.0);
	set_cone_uv(rec, cone);
	rec->v_vector = unit_vector(tip_to_intersection);
	rec->u_vector = cross(tip_to_intersection, outward_normal);
	if (cone->texture.bump_map)
		rec->normal = bump_map(rec, cone->texture.bump_map);
}

static void	get_albedo(t_cone *cone, t_hit_record *rec)
{
	if (cone->texture.type == SOLID)
	{
		rec->albedo = cone->albedo;
		return ;
	}
	if (cone->texture.type == CHECKER)
	{
		rec->albedo = get_cone_checkered_color(rec, cone);
		return ;
	}
	if (cone->texture.type == PNG_FILE)
	{
		rec->albedo = get_texture_color(cone->texture.texture_obj,
				rec->u, rec->v);
		return ;
	}
}

void	set_cone_rec(t_hit_record *rec, t_cone *cone, t_ray *ray, double t)
{
	rec->point = ray_at(*ray, t);
	rec->material1 = cone->phong.material1;
	rec->emitted = cone->phong.emitted;
	rec->k_s = cone->phong.k_s;
	rec->k_d = cone->phong.k_d;
	set_cone_face_normal(rec, ray, cone);
	get_albedo(cone, rec);
}
