/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/08 04:00:47 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void initialize_coordinate(t_vec3 *coord, char *value_param)
{
	char	**values;

	values = ft_split(value_param, ',');
	coord->x = ft_atod(values[0]);
	coord->y = ft_atod(values[1]);
	coord->z = ft_atod(values[2]);
	free_split(values);
}

// Initialize Ambient light
void	initialize_ambient(t_master *m, char **params)
{
	m->ambient.brightness = ft_atod(params[1]);
	initialize_coordinate(&m->ambient.color, params[2]);
}

// Initialize Camera
void	initialize_camera(t_master *m, char **params)
{
	initialize_coordinate(&m->camera.camera_center, params[1]);
	initialize_coordinate(&m->camera.look_at, params[2]);
	m->camera.hfov = ft_atod(params[3]);
}

// Initialize Lights
// TODO: Add several lights and add orientation to lights
void	initialize_lights(t_master *m, char **params)
{
	int i;
	int j;
	t_light *light;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	while ((m->objects_count)[i] > 0)
	{
		light = &((m->lights)[j]);
		initialize_coordinate(&light->point, params[1]);
		light->brightness = ft_atod(params[2]);
		initialize_coordinate(&light->color, params[3]);
		(m->objects_count)[i] -= 1;
	}
}

// Initialize Spheres
void	initialize_spheres(t_master *m, char **params)
{
	int i;
	int j;
	t_sphere *sphere;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	while ((m->objects_count)[i] > 0)
	{
		sphere = &((m->spheres)[j]);
		initialize_coordinate(&sphere->origin, params[1]);
		sphere->radius = ft_atod(params[2]) / 2;
		initialize_coordinate(&sphere->albedo, params[3]);
		// texture name
		// if checker init colors
		// k_s
		// k_d
		(m->objects_count)[i] -= 1;
	}
}

void initialize_object(t_master *m, char **params)
{
	// Ambient light init
	if (ft_strncmp(params[0], "A", 2) == 0)
		initialize_ambient(m, params);
	// Camera init
	else if (ft_strncmp(params[0], "C", 2) == 0)
		initialize_camera(m, params);
	// Lights init
	else if (ft_strncmp(params[0], "L", 2) == 0)
		initialize_lights(m, params);
	// Sphere init
	else if (ft_strncmp(params[0], "sp", 3) == 0)
		initialize_spheres(m, params);
/* 	// Plane init
	else if (ft_strncmp(params[0], "pl", 3) == 0)
		initialize_planes(m, params);
	// Cylinder init
	else if (ft_strncmp(params[0], "cy", 3) == 0)
		initialize_cylinders(m, params); */
}

void initialize_objects(t_master *m, int fd)
{
	char		*line;
	int			i;
	char		**params;

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (prepare_line(&line, fd) == EMPTY_LINE)
			continue ;
		printf("%d. %s\n", ++i, line);
		params = ft_split(line, ' ');
		initialize_object(m, params);

		free(line);
		free_split(params);
		line = get_next_line(fd);
	}
}

void initialize_scene(t_master *m, int fd)
{
	// (void)fd;

	// Init camera
	ft_bzero(&(m->camera), sizeof(m->camera));
	m->camera.focal_length = 1.0;
	m->camera.background_color = init_vec3(0, 0, 0);

	// m->camera.camera_center = init_vec3(0, 2, 2);
	// m->camera.hfov = 120;
	// m->camera.look_at = init_vec3(0, 0, -1);

	initialize_objects(m, fd);

}

void initialize_master_struct(t_master *m, const char *ids[])
{
	ft_bzero(m, sizeof(*m));
	m->samples_per_pixel = 300;
	m->max_depth = 4;
	m->ids = ids;
}

int	initialize(t_master *m, mlx_t *mlx, const char *argv[])
{
	int fd;


	printf("FFFFirst element: %s\n", m->ids[0]);
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

	initialize_scene(m, fd);


	close(fd);

	return (EXIT_SUCCESS);
}
