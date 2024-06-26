/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_objects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:45:54 by vshchuki          #+#    #+#             */
/*   Updated: 2024/05/17 12:58:09 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	validate_ambient_light(char **value_params)
{
	if (str_array_length(value_params) != 2)
		return (print_error("Ambient light must have 2 parameters."));
	if (validate_0_to_1(value_params[0]))
		return (EXIT_FAILURE);
	if (validate_rgb(value_params[1]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_camera(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Camera must have 3 parameters."));
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	if (validate_orientation(value_params[1]))
		return (EXIT_FAILURE);
	if (validate_param(value_params[2], validate_int_str, init_interval(0, 180),
			"Camera FOV is not in [0, 180]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_light(char **value_params)
{
	if (str_array_length(value_params) != 3)
		return (print_error("Light must have 3 parameters."));
	if (validate_position(value_params[0]))
		return (EXIT_FAILURE);
	if (validate_param(value_params[1], validate_f_str, init_interval(0.0, 1.0),
			"Light brightness ratio is out of range."))
		return (EXIT_FAILURE);
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

	valid_count = 8;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		ft_printf("Error\n");
		ft_printf("Sphere must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0]) || validate_size(value_params[1])
		|| validate_rgb(value_params[2]) || validate_texture(value_params[3])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[4 + shift])
		|| validate_0_to_1(value_params[5 + shift])
		|| validate_0_to_1(value_params[6 + shift])
		|| validate_emitted(value_params[7 + shift])
		|| validate_bump_map(value_params[8 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_plane(char **value_params)
{
	int	valid_count;
	int	shift;

	valid_count = 8;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		ft_printf("Error\n");
		ft_printf("Plane must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
		|| validate_orientation(value_params[1])
		|| validate_rgb(value_params[2]) || validate_texture(value_params[3])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[4 + shift])
		|| validate_0_to_1(value_params[5 + shift])
		|| validate_0_to_1(value_params[6 + shift])
		|| validate_emitted(value_params[7 + shift])
		|| validate_bump_map(value_params[8 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
