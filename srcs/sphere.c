/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 08:56:35 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 08:56:36 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_vec3	get_sphere_checkered_color(t_hit_record *rec, t_sphere *sphere)
{
	double	checker_u;
	double	checker_v;
	int		x;
	int		y;

	checker_u = rec->u * sphere->texture.checker_size_coeff * 2;
	checker_v = rec->v * sphere->texture.checker_size_coeff;
	x = floor(checker_u);
	y = floor(checker_v);
	if ((x + y) % 2)
		return (sphere->albedo);
	else
		return (sphere->texture.checker_color);
}

static void	set_sphere_face_normal(t_hit_record *rec,
			t_ray *r, t_sphere *sphere)
{
	int		front_face;
	t_vec3	outward_normal;

	outward_normal = vec3_div_d(vec3_minus_vec3(rec->point,
				sphere->origin), sphere->radius);
	front_face = dot(r->direction, outward_normal) < 0;
	if (front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_times_d(outward_normal, -1.0);
	get_sphere_uv(outward_normal, &rec->u, &rec->v);
	rec->u_vector = init_vec3(1, 0, 0);
	rec->v_vector = init_vec3(0, 1, 0);
	if (sphere->texture.bump_map)
		rec->normal = bump_map(rec, sphere->texture.bump_map);
}

static void	get_albedo(t_sphere *sphere, t_hit_record *rec)
{
	if (sphere->texture.type == SOLID)
	{
		rec->albedo = sphere->albedo;
		return ;
	}
	if (sphere->texture.type == CHECKER)
	{
		rec->albedo = get_sphere_checkered_color(rec, sphere);
		return ;
	}
	if (sphere->texture.type == PNG_FILE)
	{
		rec->albedo = get_texture_color(sphere->texture.texture_obj,
				rec->u, rec->v);
		return ;
	}
}

void	set_sphere_rec(t_hit_record *rec, t_sphere *sphere,
			t_ray *ray, double t)
{
	rec->point = ray_at(*ray, t);
	rec->material1 = sphere->phong.material1;
	rec->emitted = sphere->phong.emitted;
	rec->k_s = sphere->phong.k_s;
	rec->k_d = sphere->phong.k_d;
	set_sphere_face_normal(rec, ray, sphere);
	get_albedo(sphere, rec);
}
