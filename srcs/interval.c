/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interval.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:40:10 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:40:11 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_interval	init_interval(double min, double max)
{
	t_interval	ret;

	ret.min = min;
	ret.max = max;
	return (ret);
}

int	contains(t_interval i, double x)
{
	return (x <= i.max && x >= i.min);
}

int	surrounds(t_interval i, double x)
{
	return (i.min < x && x < i.max);
}

double	clamp(t_interval i, double x)
{
	if (x < i.min)
		return (i.min);
	if (x > i.max)
		return (i.max);
	return (x);
}
