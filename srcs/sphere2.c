/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:36:48 by vshchuki          #+#    #+#             */
/*   Updated: 2024/05/15 16:42:29 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	hit_sphere(t_ray *ray, t_interval t_minmax,
			double *t, t_sphere *sphere)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	discriminant;
	double	root;

	oc = vec3_minus_vec3(ray->origin, sphere->origin);
	a = vec3length_squared(ray->direction);
	half_b = dot(oc, ray->direction);
	discriminant = half_b * half_b - a
		* (vec3length_squared(oc) - sphere->radius * sphere->radius);
	if (discriminant < 0)
		return (0);
	root = (-1.0 * half_b - sqrt(discriminant)) / a;
	if (!surrounds(t_minmax, root))
	{
		root = (-1.0 * half_b + sqrt(discriminant)) / a;
		if (!surrounds(t_minmax, root))
			return (0);
	}
	*t = root;
	return (1);
}
