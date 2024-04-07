/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_param_spec.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:34 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/07 22:59:36 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Position
 */
int validate_position(char *value_param)
{
	if (validate_param(value_param, validate_f_str, init_interval(-__DBL_MAX__, __DBL_MAX__),
			"Position is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * Camera orientation [-1.0, 1.0]
 */
int validate_orientation(char *value_param)
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
	if (validate_param(value_param, validate_f_str, init_interval(0.0, __DBL_MAX__),
		"Size parameter is out of range."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * RGB Colors range [0, 255]
 */
int	validate_rgb(char *value_param)
{
	if (validate_param(value_param, validate_int_str, init_interval(0, 255),
			"RGB color is not in [0, 255]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_texture(char *value_param)
{
	size_t str_len;

	str_len = ft_strlen(value_param);
	if (ft_strncmp(value_param, "solid", str_len + 1) == 0)
		return (EXIT_SUCCESS);
	if (ft_strncmp(value_param, "checker", str_len + 1) == 0)
		return (EXIT_SUCCESS);
	if (ft_strncmp(value_param + str_len - 4, ".png", 5) == 0)
		return (EXIT_SUCCESS);
	return (print_error("Texture not implemented."));
}
