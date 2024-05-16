/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:43:01 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:43:03 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	pixel_sample_square(t_camera *c)
{
	double	px;
	double	py;

	px = -0.5 + random_double();
	py = -0.5 + random_double();
	return (vec3_plus_vec3(vec3_times_d(c->pixel_delta_u, px),
			vec3_times_d(c->pixel_delta_v, py)));
}

t_ray	get_ray(t_camera *c, int i, int j)
{
	t_vec3	pixel_center;
	t_vec3	pixel_sample;
	t_ray	ret;

	pixel_center = vec3_plus_vec3(c->pixel00_loc,
			vec3_plus_vec3(vec3_times_d(c->pixel_delta_u, i * 1.0),
				vec3_times_d(c->pixel_delta_v, j * 1.0)));
	pixel_sample = vec3_plus_vec3(pixel_center, pixel_sample_square(c));
	ret.origin = c->camera_center;
	ret.direction = vec3_minus_vec3(pixel_sample, ret.origin);
	return (ret);
}

t_vec3	ray_at(t_ray r, double t)
{
	return (vec3_plus_vec3(r.origin, vec3_times_d(r.direction, t)));
}

t_ray	init_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray	ret;

	ret.origin = origin;
	ret.direction = direction;
	return (ret);
}
