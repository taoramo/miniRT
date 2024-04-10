/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_param_spec2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:31 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/10 14:35:22 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	checker_shift(char **value_params)
{
	int	shift;

	shift = 0;
	if (index_of(value_params, "checker") != -1)
		shift = 1;
	return (shift);
}

int	validate_checker_rgbs(char **value_params)
{
	int	checker_ind;

	checker_ind = index_of(value_params, "checker");
	if (checker_ind != -1)
	{
		if (validate_rgb(value_params[checker_ind + 1]))
			return (EXIT_FAILURE);
		// if (validate_rgb(value_params[checker_ind + 2])) // Remove it, we only have one color for checker
		// 	return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	validate_0_to_1(char *value_param)
{
	if (validate_f_str(value_param) || validate_f_range(value_param, 0, 1,
			"Value not in [0.0, 1.0]."))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int validate_emitted(char *value_param)
{
	if (validate_three_tuple_size(value_param) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (validate_param(value_param, validate_f_str, init_interval(0, __DBL_MAX__),
		"Emit is not in [0, INFINITY].")) // NOTE: Validate three tuple can be also used for a number
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate_bump_map(char *value_param)
{
	size_t str_len;

	if (!value_param)
		return (EXIT_SUCCESS);
	str_len = ft_strlen(value_param);
	if (ft_strncmp(value_param + str_len - 4, ".png", 5) != 0)
		return (print_error("Bump map should be a png file."));
	return (EXIT_SUCCESS);
}
