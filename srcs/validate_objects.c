#include "miniRT.h"

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
	int	shift;

	valid_count = 6;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		printf("Sphere must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0]) || validate_size(value_params[1])
		|| validate_rgb(value_params[2]) || validate_texture(value_params[3])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[4 + shift])
		|| validate_0_to_1(value_params[5 + shift])
		|| validate_bump_map(value_params[6 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_plane(char **value_params)
{
	int	valid_count;
	int	shift;

	valid_count = 6;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		printf("Plane must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
		|| validate_orientation(value_params[1])
		|| validate_rgb(value_params[2]) || validate_texture(value_params[3])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[4 + shift])
		|| validate_0_to_1(value_params[5 + shift])
		|| validate_bump_map(value_params[6 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_cylinder(char **value_params)
{
	int	valid_count;
	int	shift;

	valid_count = 8;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		printf("Cylinder must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
		|| validate_orientation(value_params[1])
		|| validate_size(value_params[2]) || validate_size(value_params[3])
		|| validate_rgb(value_params[4]) || validate_texture(value_params[5])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[6 + shift])
		|| validate_0_to_1(value_params[7 + shift])
		|| validate_bump_map(value_params[8 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
