/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:18:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:18:52 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Initialize Ambient light
void	initialize_ambient(t_master *m, char **params)
{
	t_vec3	color;

	initialize_coordinate(&color, params[2]);
	m->camera.background_color = vec3_times_d(color, ft_atod(params[1]));
	m->camera.background_color = vec3_div_d(m->camera.background_color, 255.0);
}

// Initialize Camera
void	initialize_camera(t_master *m, char **params)
{
	initialize_coordinate(&m->camera.camera_center, params[1]);
	initialize_coordinate(&m->camera.look_at, params[2]);
	m->camera.look_at = unit_vector(m->camera.look_at);
	m->camera.hfov = ft_atod(params[3]);
}

// Initialize Light
void	initialize_light(t_master *m, char **params)
{
	int		i;
	int		j;
	t_light	*light;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	light = &((m->lights)[j]);
	initialize_coordinate(&light->point, params[1]);
	initialize_coordinate(&light->color, params[3]);
	// Apply brightness to color
	light->color = vec3_times_d(light->color, ft_atod(params[2]) / 255.0);
	(m->objects_count)[i] -= 1;
}
