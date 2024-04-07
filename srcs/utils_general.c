/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 23:00:03 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/07 23:00:04 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int index_of(char **arr, char *str)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(arr[i], str, ft_strlen(arr[i]) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}
