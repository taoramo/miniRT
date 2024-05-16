/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:39:33 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:39:35 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	check_root(double root, t_ray *ray, t_cylinder *cylinder)
{
	double	m;
	t_vec3	oc;

	oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center,
				vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	m = dot(ray->direction, cylinder->axisnormal)
		* root + dot(oc, cylinder->axisnormal);
	if (surrounds(init_interval(0, cylinder->height), m))
		return (1);
	else
		return (0);
}

static int	check_which_root(t_hit_cylinder *info, double *t)
{
	int	root1_is_valid;
	int	root2_is_valid;

	root1_is_valid = 0;
	root2_is_valid = 0;
	if (surrounds(info->t_minmax, info->root1)
		&& check_root(info->root1, info->ray, info->cylinder))
		root1_is_valid = 1;
	if (surrounds(info->t_minmax, info->root2)
		&& check_root(info->root2, info->ray, info->cylinder))
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

int	hit_cylinder(t_ray *ray, t_interval t_minmax,
				double *t, t_cylinder *cylinder)
{
	t_hit_cylinder	info;

	info.ray = ray;
	info.cylinder = cylinder;
	info.t_minmax = t_minmax;
	info.oc = vec3_minus_vec3(ray->origin, vec3_minus_vec3(cylinder->center,
				vec3_times_d(cylinder->axisnormal, cylinder->height / 2)));
	info.a = vec3length_squared(ray->direction) - dot(ray->direction,
			cylinder->axisnormal) * dot(ray->direction, cylinder->axisnormal);
	if (fabs(info.a) < EPSILON)
		return (0);
	info.half_b = dot(info.oc, ray->direction) - dot(ray->direction,
			cylinder->axisnormal) * dot(info.oc, cylinder->axisnormal);
	info.c = vec3length_squared(info.oc) - dot(info.oc, cylinder->axisnormal)
		* dot(info.oc, cylinder->axisnormal)
		- cylinder->radius * cylinder->radius;
	info.discriminant = info.half_b * info.half_b - info.a * info.c;
	if (info.discriminant < 0)
		return (0);
	info.root1 = (-1 * info.half_b - sqrt(info.discriminant)) / info.a;
	info.root2 = (-1.0 * info.half_b + sqrt(info.discriminant)) / info.a;
	return (check_which_root(&info, t));
}
