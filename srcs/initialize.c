/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/08 01:35:05 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void initialize_scene(t_master *m, t_camera *camera)
{
	// Init master struct
	ft_bzero(m, sizeof(*m));
	m->samples_per_pixel = 300;
	m->max_depth = 4;

	// Init camera
	ft_bzero(camera, sizeof(*camera));
	m->camera = camera;
	m->camera->hfov = 120;
	m->camera->focal_length = 1.0;
	m->camera->camera_center = init_vec3(0, 2, 2);
	m->camera->look_at = init_vec3(0, 0, -1);
	m->camera->background_color = init_vec3(0, 0, 0);

	// Init spheres
	

	// Init planes


	// Init cylinders


	// Init cones

}

int	initialize(t_master *m, t_camera *camera, mlx_t *mlx, const char *argv[])
{
	(void)camera;
	int fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}

	// Init mlx
	mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
	if (!mlx)
		return (print_error("MLX42 init failed"));

	// Test object count
	int i = 0;
	while (i < N_OBJECT_TYPES)
	{
		printf("%d : %s\n", i, m->ids[i]);
		i++;
	}

	// initialize_scene(&m, &camera);


	close(fd);

	return (EXIT_SUCCESS);
}
