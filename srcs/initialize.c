/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/08 22:56:51 by vshchuki         ###   ########.fr       */
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
	m->ambient.brightness = ft_atod(params[1]);
	initialize_coordinate(&m->ambient.color, params[2]); // ?
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
	printf("This i: %d\n", i);
	j = (m->objects_count)[i] - 1;
	printf("This j: %d\n", j);
	light = &((m->lights)[j]);
	initialize_coordinate(&light->point, params[1]);
	// light->brightness = ft_atod(params[2]); // we apply it to the color
	initialize_coordinate(&light->color, params[3]);
	// Apply brightness to color
	light->color = vec3_times_d(light->color, ft_atod(params[2])); // Should it be albedo alike format?
	(m->objects_count)[i] -= 1;
}

int	get_texture_type(char *text_type, bool is_bump)
{
	int	str_len;

	str_len = ft_strlen(text_type);
	if (ft_strncmp(text_type, "solid", str_len + 1) == 0)
		return (solid);
	else if (ft_strncmp(text_type, "checker", str_len + 1) == 0)
		return (checker);
	else if (ft_strncmp(text_type + str_len - 4, ".png", 5) == 0)
	{
		if (is_bump)
			return (bump_map);
		return (texture);
	}
	return (solid);
}

// Initialize Sphere
void	initialize_sphere(t_master *m, char **params)
{
	int			i;
	int			j;
	t_sphere	*sphere;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	sphere = &((m->spheres)[j]);
	initialize_coordinate(&sphere->origin, params[1]);
	sphere->radius = ft_atod(params[2]) / 2;
	initialize_coordinate(&sphere->albedo, params[3]);
	sphere->albedo = vec3_div_d(sphere->albedo, 255.0);
	// Texture
	sphere->texture_type = get_texture_type(params[4], false);
	shift = 0;
	if (sphere->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&sphere->checker_color, params[5]);
		sphere->checker_color = vec3_div_d(sphere->checker_color, 255.0);
		sphere->checkered = 1;
		sphere->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	// k_s
	sphere->k_s = ft_atod(params[5 + shift]);
	// k_d
	sphere->k_d = ft_atod(params[6 + shift]);
	(m->objects_count)[i] -= 1;
}

// Initialize Plane
void	initialize_plane(t_master *m, char **params)
{
	int			i;
	int			j;
	t_plane		*plane;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	plane = &((m->planes)[j]);
	// Init point
	initialize_coordinate(&plane->point, params[1]);
	// Init normal
	initialize_coordinate(&plane->normal, params[2]);
	plane->normal = unit_vector(plane->normal);
	// Color
	initialize_coordinate(&plane->albedo, params[3]);
	plane->albedo = vec3_div_d(plane->albedo, 255.0);
	// Texture
	plane->texture_type = get_texture_type(params[4], false);
	shift = 0;
	if (plane->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&plane->checker_color, params[5]);
		plane->checker_color = vec3_div_d(plane->checker_color, 255.0);
		plane->checkered = 1;
		plane->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	// k_s
	plane->k_s = ft_atod(params[5 + shift]);
	// k_d
	plane->k_d = ft_atod(params[6 + shift]);
	(m->objects_count)[i] -= 1;
}

void	initialize_cylinder(t_master *m, char **params)
{
	int			i;
	int			j;
	t_cylinder	*cylinder;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	cylinder = &((m->cylinders)[j]);
	// Init center
	initialize_coordinate(&cylinder->center, params[1]);
	initialize_coordinate(&cylinder->axisnormal, params[2]);
	cylinder->axisnormal = unit_vector(cylinder->axisnormal);
	cylinder->radius = ft_atod(params[3]) / 2;
	cylinder->height = ft_atod(params[4]);
	initialize_coordinate(&cylinder->albedo, params[5]);
	cylinder->albedo = vec3_div_d(cylinder->albedo, 255.0);
	// Texture
	cylinder->texture_type = get_texture_type(params[6], false);
	shift = 0;
	if (cylinder->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&cylinder->checker_color, params[7]);
		cylinder->checker_color = vec3_div_d(cylinder->checker_color, 255.0);
		cylinder->checkered = 1;
		cylinder->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	// k_s
	cylinder->k_s = ft_atod(params[7 + shift]);
	// k_d
	cylinder->k_d = ft_atod(params[8 + shift]);
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
/* 	// Cone init
	else if (ft_strncmp(params[0], "co", 3) == 0)
		initialize_cylinder(m, params); */
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
	(void)fd;
	// Init camera defaults
	ft_bzero(&(m->camera), sizeof(m->camera));
	m->camera.focal_length = 1.0;
	m->camera.background_color = init_vec3(0, 0, 0); // should it be albedo alike format? probably should come from ambient light

	// These are defined from initialize_object:
	// m->camera.camera_center = init_vec3(0, 2, 2);
	// m->camera.hfov = 120;
	// m->camera.look_at = init_vec3(0, 0, -1);

	// Save objects count
	m->n_lights = m->objects_count[2];
	m->n_spheres = m->objects_count[3];
	m->n_planes = m->objects_count[4];
	m->n_cylinders = m->objects_count[5];
	/* m->n_cones = m->objects_count[6]; */
	initialize_objects(m, fd);

}

void initialize_master_struct(t_master *m, const char *ids[])
{
	ft_bzero(m, sizeof(*m));
	m->samples_per_pixel = 10; //300
	m->max_depth = 4;
	m->ids = ids;
}

int	initialize(t_master *m, mlx_t **mlx, const char *argv[])
{
	(void)m;
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
