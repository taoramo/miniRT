/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3b.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:44:05 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:44:06 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

double	vec3length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

double	vec3length(t_vec3 v)
{
	return (sqrt(vec3length_squared(v)));
}

t_vec3	vec3_div_d(t_vec3 u, double t)
{
	t_vec3	ret;

	ret.x = u.x / t;
	ret.y = u.y / t;
	ret.z = u.z / t;
	return (ret);
}

t_vec3	unit_vector(t_vec3 v)
{
	return (vec3_div_d(v, vec3length(v)));
}

t_vec3	init_vec3(double x, double y, double z)
{
	t_vec3	ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}
