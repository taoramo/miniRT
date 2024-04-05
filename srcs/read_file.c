#include "miniRT.h"

bool is_capital(char c)
{
	return (c >= 'A' && c <= 'Z');
}

/**
 * Checks for \t (ht) - tab, \n (nl) - new line, \v (vt) vertical tab, 
 * \f (np) - feed form aka new page, \r (cr) - carriage return
*/
bool is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	str_array_length(char **str)
{
	int	length;

	length = 0;
	while (*str)
	{
		length++;
		str++;
	}
	return (length);
}

int print_error(char *err)
{
	printf("Error:\n%s\n", err);
	return (EXIT_FAILURE);
}

void free_split(char **split)
{
	char **tmp;

	tmp = split;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(split);
}

void	replace_whitespaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			line[i] = ' ';
		i++;
	}
}

// Identifier
// A, C, L, sp, pl, cy

// A 0.2 255,255,255
// C -50.0,0,20 0,0,1 70
// L -40.0,50.0,0.0 0.6 10,0,255
// sp 0.0,0.0,20.6 12.6 10,0,255
// pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
// cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255

/**
 * Returns failure on floats like .123, +15.123
*/
int	validate_int_str(char *int_str)
{
	int		i;
	char	*minus_pos;
	int		at_least_one_digit;

	minus_pos = ft_strchr(int_str, '-');
	// Only one '-' allowed
	if (minus_pos != ft_strrchr(int_str, '-'))
		return (print_error("Int string has repeated minus char."));
	// Minus can be only the first character
	if (minus_pos != NULL && minus_pos != int_str)
		return (print_error("Minus is not the first character."));
	// Only digits and minus allowed
	i = 0;
	at_least_one_digit = 0;
	while (int_str[i])
	{
		if (!ft_isdigit(int_str[i]) && int_str[i] != '-')
			return (print_error("Forbidden char in int string."));
		else if (ft_isdigit(int_str[i]))
			at_least_one_digit = 1;
		i++;
	}
	if (!at_least_one_digit)
		return (print_error("Int string must contain at least one digit."));
	return (EXIT_SUCCESS);
}

/**
 * Returns failure on floats like .123, +15.123
*/
int	validate_f_str(char *f_str)
{
	int		i;
	char	*dot_pos;
	char	*minus_pos;
	int		at_least_one_digit;

	dot_pos = ft_strchr(f_str, '.');
	minus_pos = ft_strchr(f_str, '-');
	// Only one '.' and '-' allowed
	if (dot_pos != ft_strrchr(f_str, '.')
		|| minus_pos != ft_strrchr(f_str, '-'))
		return (print_error("Float string has repeated non-digital char."));
	// Dot can not be in the beginning or end
	if (dot_pos != NULL && (dot_pos == f_str
			|| *dot_pos == f_str[ft_strlen(f_str) - 1]))
		return (print_error("Invalid dot position in float string."));
	// Digit must be before the dot
	if (dot_pos != NULL && !ft_isdigit(*(dot_pos - 1)))
		return (print_error("Digit must be before the dot."));
	// Minus can be only the first character
	if (minus_pos != NULL && minus_pos != f_str)
		return (print_error("Minus is not the first character."));
	// Only digits, dot and minus allowed
	i = 0;
	at_least_one_digit = 0;
	while (f_str[i])
	{
		if (!ft_isdigit(f_str[i])
			&& f_str[i] != '.' && f_str[i] != '-')
			return (print_error("Forbidden chars in float string."));
		else if (ft_isdigit(f_str[i]))
			at_least_one_digit = 1;
		i++;
	}
	if (!at_least_one_digit)
		return (print_error("Float string must contain at least one digit."));
	return (EXIT_SUCCESS);
}

int validate_f_range(char *f_str, float min, float max, char *err)
{
	if (ft_atod(f_str) < min || ft_atod(f_str) - max > EPSILON)
		return (print_error(err));
	return (EXIT_SUCCESS);
}

int	validate_three_tuple(char *value_param, t_validate_str f, t_interval range, char *err)
{
	char	**tuple;
	char	**temp;

	tuple = ft_split(value_param, ',');
	temp = tuple;
	while (*tuple)
	{
		if (f(*tuple) || validate_f_range(*tuple, range.min, range.max, err))
			return (EXIT_FAILURE);
		tuple++;
	}
	free_split(temp);
	return (EXIT_SUCCESS);
}

/**
 * Position
 */
int validate_position(char *value_param)
{
	if (validate_three_tuple(value_param, validate_f_str, init_interval(-__DBL_MAX__, __DBL_MAX__),
			"Position is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Camera orientation [-1.0, 1.0]
 */
int validate_orientation(char *value_param)
{
	if (validate_three_tuple(value_param, validate_f_str, init_interval(-1.0, 1.0),
			"Orientation is not in [-1, 1]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Size [0.0, INFINITY]
 */
int	validate_size(char *value_param)
{
	if (validate_three_tuple(value_param, validate_f_str, init_interval(0.0, __DBL_MAX__),
		"Size parameter is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * RGB Colors range [0, 255]
 */
int	validate_rgb(char *value_param)
{
	if (validate_three_tuple(value_param, validate_int_str, init_interval(0, 255),
			"RGB color not in [0, 255]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_ambient_light(char **value_params)
{
	if (str_array_length(value_params) != 2)
		return (print_error("Ambient light must have 2 parameters."));
	// Ambient light ratio [0.0, 1.0]
	if (validate_f_str(value_params[0]) || validate_f_range(value_params[0], 0, 1,
			"Ambient light intensity not in [0.0, 1.0]."))
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
	if (validate_three_tuple(value_params[2], validate_int_str, init_interval(0, 180),
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
	if (validate_three_tuple(value_params[1], validate_f_str, init_interval(0.0, 1.0),
			"Light brightness ratio is out of range."))
		return (EXIT_FAILURE);
	// RGB Colors range [0, 255]
	if (validate_rgb(value_params[2]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_sphere(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Sphere must have 3 parameters."));
	// Sphere position x,y,z
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	// Sphere diameter [0.0, INFINITY]
	if (validate_size(value_params[1]))
		return (EXIT_FAILURE);
	// RGB Colors range [0, 255]
	if (validate_rgb(value_params[2]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_plane(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Plane must have 3 parameters."));
	// Plane position
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	// Plane orientation [-1.0, 1.0]
	if (validate_orientation(value_params[1]))
		return (EXIT_FAILURE);
	// RGB Colors range [0, 255]
	if (validate_rgb(value_params[2]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_cylinder(char **value_params)
{
	if (str_array_length(value_params) != 5)
		return (print_error("Cylinder must have 5 parameters."));
	// Plane position
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	// Plane orientation [-1.0, 1.0]
	if (validate_orientation(value_params[1]))
		return (EXIT_FAILURE);
	// diameter
	if (validate_size(value_params[2]))
		return (EXIT_FAILURE);
	// height
	if (validate_size(value_params[3]))
		return (EXIT_FAILURE);
	// color
	if (validate_rgb(value_params[4]))
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

// void	parse(t_master m, int objects_count[], int fd)
int	parse(int objects_count[], int fd)
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
		printf("%d. ", ++i);
		printf("%s\n", line);

		if (validate_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
		{
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	printf("Scene verified.\n");
	return (EXIT_SUCCESS);
}

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
	if (parse(objects_count, fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(argv[1]);
	// Test object count
	// int i = 0;
	// while (i < N_OBJECT_TYPES)
	// {
	// 	printf("%d : %d\n", i, objects_count[i]);
	// 	i++;
	// }

	return (EXIT_SUCCESS);
}
