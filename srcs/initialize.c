/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:46:04 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/11 03:22:42 by vshchuki         ###   ########.fr       */
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
	t_vec3 color;

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

int	get_texture_type(char *text_type)
{
	int	str_len;

	str_len = ft_strlen(text_type);
	if (ft_strncmp(text_type, "solid", str_len + 1) == 0)
		return (solid);
	else if (ft_strncmp(text_type, "checker", str_len + 1) == 0)
		return (checker);
	else if (ft_strncmp(text_type + str_len - 4, ".png", 5) == 0)
		return (texture);
	return (solid);
}

// Initialize Sphere
void	initialize_sphere(t_master *m, char **params)
{
	int			i;
	int			j;
	t_sphere	*sphere;
	int			shift;
	char		*texture_path;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	sphere = &((m->spheres)[j]);
	sphere->bump_map = NULL;
	sphere->texture = NULL;
	sphere->checkered = 0;
	initialize_coordinate(&sphere->origin, params[1]);
	sphere->radius = ft_atod(params[2]) / 2;
	initialize_coordinate(&sphere->albedo, params[3]);
	sphere->albedo = vec3_div_d(sphere->albedo, 255.0);
	// Texture
	sphere->texture_type = get_texture_type(params[4]);
	shift = 0;
	if (sphere->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&sphere->checker_color, params[5]);
		sphere->checker_color = vec3_div_d(sphere->checker_color, 255.0);
		sphere->checkered = 1;
		sphere->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	if (sphere->texture_type == texture)
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[4]);
		sphere->texture = mlx_load_png(texture_path);
		free(texture_path);
	}
	// k_s
	sphere->k_s = ft_atod(params[5 + shift]);
	// k_d
	sphere->k_d = ft_atod(params[6 + shift]);
	// fuzz
	sphere->material1 = ft_atod(params[7 + shift]);
	// Emission
	initialize_coordinate(&sphere->emitted , params[8 + shift]);
	// Bump map
	if (params[9 + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[9 + shift]);
		sphere->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
	(m->objects_count)[i] -= 1;
}

// Initialize Plane
void	initialize_plane(t_master *m, char **params)
{
	int			i;
	int			j;
	t_plane		*plane;
	int			shift;
	char		*texture_path;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	plane = &((m->planes)[j]);
	plane->bump_map = NULL;
	plane->texture = NULL;
	plane->checkered = 0;
	// Init point
	initialize_coordinate(&plane->point, params[1]);
	// Init normal
	initialize_coordinate(&plane->normal, params[2]);
	plane->normal = unit_vector(plane->normal);
	// Color
	initialize_coordinate(&plane->albedo, params[3]);
	plane->albedo = vec3_div_d(plane->albedo, 255.0);
	// Texture
	plane->texture_type = get_texture_type(params[4]);
	shift = 0;
	if (plane->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&plane->checker_color, params[5]);
		plane->checker_color = vec3_div_d(plane->checker_color, 255.0);
		plane->checkered = 1;
		plane->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	if (plane->texture_type == texture)
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[4]);
		plane->texture = mlx_load_png(texture_path);
		free(texture_path);
	}
	// k_s
	plane->k_s = ft_atod(params[5 + shift]);
	// k_d
	plane->k_d = ft_atod(params[6 + shift]);
	// fuzz
	plane->material1 = ft_atod(params[7 + shift]);
	// Emission
	initialize_coordinate(&plane->emitted , params[8 + shift]);
	// Bump map
	if (params[9 + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[9 + shift]);
		plane->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
	(m->objects_count)[i] -= 1;
}

void	initialize_cylinder(t_master *m, char **params)
{
	int			i;
	int			j;
	t_cylinder	*cylinder;
	int			shift;
	char		*texture_path;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	cylinder = &((m->cylinders)[j]);
	cylinder->bump_map = NULL;
	cylinder->texture = NULL;
	cylinder->checkered = 0;
	// Init center
	initialize_coordinate(&cylinder->center, params[1]);
	initialize_coordinate(&cylinder->axisnormal, params[2]);
	cylinder->axisnormal = unit_vector(cylinder->axisnormal);
	cylinder->radius = ft_atod(params[3]) / 2;
	cylinder->height = ft_atod(params[4]);
	initialize_coordinate(&cylinder->albedo, params[5]);
	cylinder->albedo = vec3_div_d(cylinder->albedo, 255.0);
	// Texture
	cylinder->texture_type = get_texture_type(params[6]);
	shift = 0;
	if (cylinder->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&cylinder->checker_color, params[7]);
		cylinder->checker_color = vec3_div_d(cylinder->checker_color, 255.0);
		cylinder->checkered = 1;
		cylinder->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	if (cylinder->texture_type == texture)
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[6]);
		cylinder->texture = mlx_load_png(texture_path);
		free(texture_path);
	}
	// k_s
	cylinder->k_s = ft_atod(params[7 + shift]);
	// k_d
	cylinder->k_d = ft_atod(params[8 + shift]);
	// fuzz
	cylinder->material1 = ft_atod(params[9 + shift]);
	// Emission
	initialize_coordinate(&cylinder->emitted , params[10 + shift]);
	// Bump map
	if (params[11 + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[11 + shift]);
		cylinder->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
	(m->objects_count)[i] -= 1;
}

void	initialize_cone(t_master *m, char **params)
{
	int			i;
	int			j;
	t_cone	*cone;
	int			shift;
	char		*texture_path;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	cone = &((m->cones)[j]);
	cone->bump_map = NULL;
	cone->texture = NULL;
	cone->checkered = 0;
	// Init center
	initialize_coordinate(&cone->tip, params[1]);
	initialize_coordinate(&cone->axis, params[2]);
	cone->axis = unit_vector(cone->axis);
	// cone->radius = ft_atod(params[3]) / 2;
	cone->height = ft_atod(params[3]);
	cone->angle = ft_atod(params[4]);
	initialize_coordinate(&cone->albedo, params[5]);
	cone->albedo = vec3_div_d(cone->albedo, 255.0);
	// Texture
	cone->texture_type = get_texture_type(params[6]);
	shift = 0;
	if (cone->texture_type == checker)
	{
		shift = 1;
		initialize_coordinate(&cone->checker_color, params[7]);
		cone->checker_color = vec3_div_d(cone->checker_color, 255.0);
		cone->checkered = 1;
		cone->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	if (cone->texture_type == texture)
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[6]);
		cone->texture = mlx_load_png(texture_path);
		free(texture_path);
	}
	// k_s
	cone->k_s = ft_atod(params[7 + shift]);
	// k_d
	cone->k_d = ft_atod(params[8 + shift]);
	// fuzz
	cone->material1 = ft_atod(params[9 + shift]);
	// Emission
	initialize_coordinate(&cone->emitted , params[10 + shift]);
	// Bump map
	if (params[11 + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[11 + shift]);
		cone->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
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
	(void)fd;
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
