/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:44:35 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:44:37 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	random_on_hemisphere(t_vec3	normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		return (on_unit_sphere);
	else
		return (vec3_times_d(on_unit_sphere, -1.0));
}

int	near_zero(t_vec3 vec)
{
	double	s;

	s = 0.00000001;
	return (fabs(vec.x) < s && (fabs(vec.y) < s) && (fabs(vec.z) < s));
}

t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	return (vec3_minus_vec3(v, vec3_times_d(n, 2 * dot(v, n))));
}
