/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:45:32 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 15:21:54 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	check_root_cone(double root, t_ray *ray, t_cone *cone)
{
	double	m;
	t_vec3	oc;

	oc = vec3_minus_vec3(ray->origin, cone->tip);
	m = dot(ray->direction, cone->axis)
		* root + dot(oc, cone->axis);
	if (surrounds(init_interval(-cone->height / 2, cone->height / 2), m))
		return (1);
	else
		return (0);
}

static int	check_which_root_cone(t_hit_cone *info, double *t)
{
	int	root1_is_valid;
	int	root2_is_valid;

	root1_is_valid = 0;
	root2_is_valid = 0;
	if (surrounds(info->t_minmax, info->root1)
		&& check_root_cone(info->root1, info->ray, info->cone))
		root1_is_valid = 1;
	if (surrounds(info->t_minmax, info->root2)
		&& check_root_cone(info->root2, info->ray, info->cone))
		root2_is_valid = 1;
	if (root1_is_valid && root2_is_valid)
	{
		if (info->root1 < info->root2)
			*t = info->root1;
		else
			*t = info->root2;
	}
	else if (root1_is_valid)
		*t = info->root1;
	else if (root2_is_valid)
		*t = info->root2;
	else
		return (0);
	return (1);
}

static int	calculate_roots_check(t_hit_cone *info, double *t)
{
	info->discriminant = info->half_b * info->half_b - info->a * info->c;
	if (info->discriminant < 0)
		return (0);
	info->root1 = (-info->half_b - sqrt(info->discriminant)) / info->a;
	info->root2 = (-info->half_b + sqrt(info->discriminant)) / info->a;
	if (!check_which_root_cone(info, t))
		return (0);
	return (1);
}

int	hit_cone(t_ray *ray, t_interval t_minmax, double *t, t_cone *cone)
{
	t_hit_cone	info;

	info.ray = ray;
	info.cone = cone;
	info.t_minmax = t_minmax;
	info.oc = vec3_minus_vec3(ray->origin, cone->tip);
	info.angle = degrees_to_radians(cone->angle);
	info.a = ray->direction.x * ray->direction.x - ray->direction.y
		* ray->direction.y * pow(sin(info.angle), 2) + ray->direction.z * ray->direction.z;
	info.half_b = info.oc.x * ray->direction.x - info.oc.y
		* ray->direction.y * pow(sin(info.angle), 2) + info.oc.z * ray->direction.z;
	info.c = (info.oc.x * info.oc.x - info.oc.y * info.oc.y
			* pow(sin(info.angle), 2) + info.oc.z * info.oc.z);
	if (fabs(info.a) < EPSILON && fabs(info.half_b) < EPSILON)
		return (0);
	else if (fabs(info.a) < EPSILON && fabs(info.half_b) > EPSILON)
	{
		*t = -info.c / (4 * info.half_b);
		return (1);
	}
	return (calculate_roots_check(&info, t));
}
