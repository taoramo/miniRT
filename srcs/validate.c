#include "miniRT.h"

// Identifier
// A, C, L, sp, pl, cy

// A 0.2 255,255,255
// C -50.0,0,20 0,0,1 70
// L -40.0,50.0,0.0 0.6 10,0,255
// sp 0.0,0.0,20.6 12.6 10,0,255
// pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
// cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255

int checker_shift(char **value_params)
{
	int shift;

	shift = 0;
	if (index_of(value_params, "checker") != -1)
		shift = 2;
	return (shift);	
}

int validate_checker_rgbs(char **value_params)
{
	int checker_ind;

	checker_ind = index_of(value_params, "checker");
	if (index_of(value_params, "checker") != -1)
	{
		if (checker_ind != -1 && validate_rgb(value_params[checker_ind + 1]))
			return (EXIT_FAILURE);
		if (validate_rgb(value_params[checker_ind + 2]))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int validate_0_to_1(char *value_param)
{
	if (validate_f_str(value_param) || validate_f_range(value_param, 0, 1,
			"Value not in [0.0, 1.0]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_ambient_light(char **value_params)
{
	if (str_array_length(value_params) != 2)
		return (print_error("Ambient light must have 2 parameters."));
	// Ambient light ratio [0.0, 1.0]
	if (validate_0_to_1(value_params[0]))
		return (EXIT_FAILURE);
	// RGB Colors range [0, 255]
	if (validate_rgb(value_params[1]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_camera(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Camera must have 3 parameters."));
	// Camera position x,y,z
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	// Camera orientation [-1.0, 1.0]
	if (validate_orientation(value_params[1]))
		return (EXIT_FAILURE);
	// FOV [0, 180]
	if (validate_param(value_params[2], validate_int_str, init_interval(0, 180),
		"Camera FOV is not in [0, 180].")) // NOTE: Validate three tuple can be also used for a number
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_light(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Light must have 3 parameters."));
	// Light position
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	// Light brightness ratio [0.0, 1.0]
	if (validate_param(value_params[1], validate_f_str, init_interval(0.0, 1.0),
			"Light brightness ratio is out of range."))
		return (EXIT_FAILURE);
	// RGB Colors range [0, 255]
	if (validate_rgb(value_params[2]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Example:
 * sp (x,y,z) (diameter) (RGB) (texture) (k_s) (k_d)
 * sp 0.0,0.0,20.6 12.6 10,0,255 solid 0.5 0.5
 * 
 * sp (x,y,z) (diameter) (RGB) checker (RGB) (RGB) (k_s) (k_d)
 * sp 0.0,0.0,20.6 12.6 10,0,255 checker 255,0,0 0,255,0 0.5 0.5
*/
int	validate_sphere(char **value_params)
{
	int	valid_count;
	int shift;

	valid_count = 6;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift)
	{
		printf("Sphere must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0]) || validate_size(value_params[1])
		|| validate_rgb(value_params[2]) || validate_texture(value_params[3])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[4 + shift])
		|| validate_0_to_1(value_params[5 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_plane(char **value_params)
{
	int	valid_count;
	int shift;

	valid_count = 6;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift)
	{
		printf("Plane must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
	|| validate_orientation(value_params[1]) || validate_rgb(value_params[2])
	|| validate_texture(value_params[3]) || validate_checker_rgbs(value_params)
	|| validate_0_to_1(value_params[4 + shift])
	|| validate_0_to_1(value_params[5 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_cylinder(char **value_params)
{
	int	valid_count;
	int shift;

	valid_count = 8;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift)
	{
		printf("Cylinder must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
	|| validate_orientation(value_params[1]) || validate_size(value_params[2])
	|| validate_size(value_params[3]) || validate_rgb(value_params[4])
	|| validate_texture(value_params[5]) || validate_checker_rgbs(value_params)
	|| validate_0_to_1(value_params[6])
	|| validate_0_to_1(value_params[7]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_line_parameters(char *line, t_object_type id)
{
	char	**params;
	int		is_exit_failure;

	is_exit_failure = EXIT_FAILURE;
	replace_whitespaces(line);
	params = ft_split(line, ' ');
	if (id == A)
		is_exit_failure = validate_ambient_light(params + 1);
	else if (id == C)
		is_exit_failure = validate_camera(params + 1);
	else if (id == L)
		is_exit_failure = validate_light(params + 1);
	else if (id == sp)
		is_exit_failure = validate_sphere(params + 1);
	else if (id == pl)
		is_exit_failure = validate_plane(params + 1);
	else if (id == cy)
		is_exit_failure = validate_cylinder(params + 1);
	// else if (id == co)
	// 	is_exit_failure = validate_cone(params + 1);
	free_split(params);
	return (is_exit_failure);
	// return (is_exit_failure && print_error("Invalid parameters."));
}

int	validate_unique(int objects_count[], t_object_type id)
{
	if (objects_count[id] > 1)
		return (print_error("Elements which are defined by a capital "
				"letter can only be declared once in the scene."));
	return (0);
}

int	validate_line_identifier(char *line, int objects_count[], const char *ids[])
{
	char			*id_with_space;
	t_object_type	id;

	id = 0;
	while (id < N_OBJECT_TYPES)
	{
		id_with_space = ft_strjoin(ids[id], " ");
		if (!ft_strncmp(line, id_with_space, ft_strlen(ids[id]) + 1))
		{
			objects_count[id]++;
			free(id_with_space);
			if (is_capital(ids[id][0]) &&
				validate_unique(objects_count, id) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			return (validate_line_parameters(line, id));
		}
		id++;
	}
	if (id == N_OBJECT_TYPES)
		return (print_error("Invalid identifier."));
	return (EXIT_SUCCESS);
}

int	validate_scene(int objects_count[], int fd)
{
	char	*line;
	char	*tmp;
	int		i;
	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "L", "sp", "pl", "cy"};

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		tmp = ft_strtrim(line, " \f\n\r\t\v");
		free(line);
		line = tmp;
		if (!tmp[0])
		{
			line = get_next_line(fd);
			continue ;
		}
		printf("%d. %s\n", ++i, line);

		if (validate_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
		{
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	printf("Scene validated.\n");
	return (EXIT_SUCCESS);
}

/* allocate_objects(int objects_count[])
{
	if (objects_count[A])
		allocate_ambient_light();
	if (objects_count[C])
		allocate_camera();
	if (objects_count[L])
		allocate_light();
	if (objects_count[sp])
		allocate_sphere();
	if (objects_count[pl])
		allocate_plane();
	if (objects_count[cy])
		allocate_cylinder();
	// if (objects_count[co])
	// 	allocate_cone();
} */

int	main(int argc, char const *argv[])
{
	int	objects_count[N_OBJECT_TYPES];
	ft_bzero(objects_count, sizeof(int) * N_OBJECT_TYPES);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	if (validate_scene(objects_count, fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);

	// Test object count
	// int i = 0;
	// while (i < N_OBJECT_TYPES)
	// {
	// 	printf("%d : %d\n", i, objects_count[i]);
	// 	i++;
	// }

	return (EXIT_SUCCESS);
}
