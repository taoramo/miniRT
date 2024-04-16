/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:28 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:44:15 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	print_error(char *err)
{
	printf("Error:\n");
	if (err[0])
		printf("%s\n", err);
	return (EXIT_FAILURE);
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

int	checker_shift(char **value_params)
{
	int	shift;

	shift = 0;
	if (index_of(value_params, "checker") != -1)
		shift = 1;
	return (shift);
}
