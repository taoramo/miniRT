/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_param_spec2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:31 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/08 15:34:26 by vshchuki         ###   ########.fr       */
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
