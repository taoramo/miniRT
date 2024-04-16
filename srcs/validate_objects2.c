/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_objects2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:45:14 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:45:24 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	validate_cylinder(char **value_params)
{
	int	valid_count;
	int	shift;

	valid_count = 10;
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
		|| validate_0_to_1(value_params[8 + shift])
		|| validate_emitted(value_params[9 + shift])
		|| validate_bump_map(value_params[10 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_cone(char **value_params)
{
	int	valid_count;
	int	shift;

	valid_count = 10;
	shift = checker_shift(value_params);
	if (str_array_length(value_params) != valid_count + shift
		&& str_array_length(value_params) != valid_count + shift + 1)
	{
		printf("Cone must have %d parameters.\n", valid_count + shift);
		return (EXIT_FAILURE);
	}
	if (validate_position(value_params[0])
		|| validate_orientation(value_params[1])
		|| validate_size(value_params[2]) || validate_size(value_params[3])
		|| validate_rgb(value_params[4]) || validate_texture(value_params[5])
		|| validate_checker_rgbs(value_params)
		|| validate_0_to_1(value_params[6 + shift])
		|| validate_0_to_1(value_params[7 + shift])
		|| validate_0_to_1(value_params[8 + shift])
		|| validate_emitted(value_params[9 + shift])
		|| validate_bump_map(value_params[10 + shift]))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}