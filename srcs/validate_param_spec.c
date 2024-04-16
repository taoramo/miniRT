/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_param_spec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:34 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:43:39 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Position
 */
int	validate_position(char *value_param)
{
	if (validate_three_tuple_size(value_param) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (validate_param(value_param, validate_f_str,
			init_interval(-__DBL_MAX__, __DBL_MAX__),
			"Position is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Camera orientation [-1.0, 1.0]
 */
int	validate_orientation(char *value_param)
{
	if (validate_param(value_param, validate_f_str, init_interval(-1.0, 1.0),
			"Orientation is not in [-1, 1]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Size [0.0, INFINITY]
 */
int	validate_size(char *value_param)
{
	if (validate_param(value_param, validate_f_str,
			init_interval(0.0, __DBL_MAX__), "Size parameter is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_three_tuple_size(char *value_param)
{
	char	**split_param;

	split_param = ft_split(value_param, ',');
	if (str_array_length(split_param) != 3)
	{
		free_split(split_param);
		return (print_error("Three tuple must have three values."));
	}
	free_split(split_param);
	return (EXIT_SUCCESS);
}

/**
 * RGB Colors range [0, 255]
 */
int	validate_rgb(char *value_param)
{
	if (validate_three_tuple_size(value_param) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (validate_param(value_param, validate_int_str, init_interval(0, 255),
			"RGB color is not in [0, 255]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
