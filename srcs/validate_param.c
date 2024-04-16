/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_param.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:38 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:39:10 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Returns failure on floats like .123, +15.123
*/
int	validate_int_str(char *int_str)
{
	int		i;
	char	*minus_pos;
	int		at_least_one_digit;

	minus_pos = ft_strchr(int_str, '-');
	if (minus_pos != ft_strrchr(int_str, '-'))
		return (print_error("Int string has repeated minus char."));
	if (minus_pos != NULL && minus_pos != int_str)
		return (print_error("Minus is not the first character."));
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

static int	validate_f_str_char_positions(char *f_str)
{
	char	*dot_pos;
	char	*minus_pos;

	dot_pos = ft_strchr(f_str, '.');
	minus_pos = ft_strchr(f_str, '-');
	if (dot_pos != ft_strrchr(f_str, '.')
		|| minus_pos != ft_strrchr(f_str, '-'))
		return (print_error("Float string has repeated non-digital char."));
	if (dot_pos != NULL && (dot_pos == f_str
			|| *dot_pos == f_str[ft_strlen(f_str) - 1]))
		return (print_error("Invalid dot position in float string."));
	if (dot_pos != NULL && !ft_isdigit(*(dot_pos - 1)))
		return (print_error("Digit must be before the dot."));
	if (minus_pos != NULL && minus_pos != f_str)
		return (print_error("Minus is not the first character."));
	return (EXIT_SUCCESS);
}

/**
 * Validates float string.
 * Returns failure on floats like .123, +15.123
*/
int	validate_f_str(char *f_str)
{
	int		i;
	int		at_least_one_digit;

	if (validate_f_str_char_positions(f_str) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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

int	validate_f_range(char *f_str, float min, float max, char *err)
{
	if (ft_atod(f_str) < min || ft_atod(f_str) - max > EPSILON)
		return (print_error(err));
	return (EXIT_SUCCESS);
}

/**
 * Validates a parameter that is a list of values separated by commas.
 * Can validate a single param without commas.
*/
int	validate_param(char *value_param, t_val_f f, t_interval range, char *err)
{
	char	**tuple;
	char	**temp;

	tuple = ft_split(value_param, ',');
	if (!tuple)
		return (print_error("Memory allocation failed."));
	temp = tuple;
	while (*tuple)
	{
		if (f(*tuple) || validate_f_range(*tuple, range.min, range.max, err))
		{
			free_split(temp);
			return (EXIT_FAILURE);
		}
		tuple++;
	}
	free_split(temp);
	return (EXIT_SUCCESS);
}
