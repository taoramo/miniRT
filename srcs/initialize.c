/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/08 00:48:27 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	initialize(t_master *m, t_camera *camera, mlx_t *mlx)
{
	// Initialize camera
	mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
	if (!mlx)
		return (ft_error());
	ft_bzero(&m, sizeof(m));
	ft_bzero(&camera, sizeof(camera));
	m->camera = camera;
	m->camera->hfov = 120;
	m->camera->focal_length = 1.0;
	m->camera->camera_center = init_vec3(0, 2, 2);
	m->camera->look_at = init_vec3(0, 0, -1);
	// m->camera->background_color = init_vec3(0, 0, 0);
}