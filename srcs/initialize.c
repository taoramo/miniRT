/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 14:25:52 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	initialize_coordinate(t_vec3 *coord, char *value_param)
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

void	initialize_object(t_master *m, char **params)
{
	// Ambient light init
	if (ft_strncmp(params[0], "A", 2) == 0)
		initialize_ambient(m, params);
	// Camera init
	else if (ft_strncmp(params[0], "C", 2) == 0)
		initialize_camera(m, params);
 	// Lights init
	else if (ft_strncmp(params[0], "l", 2) == 0)
		initialize_light(m, params);
	// Sphere init
	else if (ft_strncmp(params[0], "sp", 3) == 0)
		initialize_sphere(m, params);
  	// Plane init
	else if (ft_strncmp(params[0], "pl", 3) == 0)
		initialize_plane(m, params);
	// Cylinder init
	else if (ft_strncmp(params[0], "cy", 3) == 0)
		initialize_cylinder(m, params);
	// Cone init
	else if (ft_strncmp(params[0], "co", 3) == 0)
		initialize_cone(m, params);
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
	// Init camera defaults
	ft_bzero(&(m->camera), sizeof(m->camera));
	m->camera.focal_length = 1.0;

	// Save objects count
	m->n_lights = m->objects_count[2];
	m->n_spheres = m->objects_count[3];
	m->n_planes = m->objects_count[4];
	m->n_cylinders = m->objects_count[5];
	m->n_cones = m->objects_count[6];
	initialize_objects(m, fd);
}

void initialize_master_struct(t_master *m, const char *ids[])
{
	ft_bzero(m, sizeof(*m));
	m->samples_per_pixel = SAMPLES_PER_PIXEL;
	m->max_depth = MAX_DEPTH;
	m->ids = ids;
}

int	initialize(t_master *m, mlx_t **mlx, const char *argv[])
{
	int fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (EXIT_FAILURE);
	}

	// Init mlx
	*mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
	if (!mlx)
		return (print_error("MLX42 init failed"));

/* 	// Test object count
	int i = 0;
	while (i < N_OBJECT_TYPES)
	{
		printf("%d : %s\n", i, m->ids[i]);
		i++;
	} */

	initialize_scene(m, fd);


	close(fd);

	return (EXIT_SUCCESS);
}
