/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_param.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:12:38 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 21:02:38 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	initialize_coordinate(t_vec3 *coord, char *value_param)
{
	char	**values;

	values = ft_split(value_param, ',');
	coord->x = ft_atod(values[0]);
	coord->y = ft_atod(values[1]);
	coord->z = ft_atod(values[2]);
	free_split(values);
}

/**
 *	@param i index of k_s without shifting (specular param) 
*/
void	initialize_phong(t_phong *phong, char **params, int i, int shift)
{
	phong->k_s = ft_atod(params[i++ + shift]);
	phong->k_d = ft_atod(params[i++ + shift]);
	phong->material1 = fmax(0.001, ft_atod(params[i++ + shift]));
	initialize_coordinate(&phong->emitted, params[i++ + shift]);
}

/**
 *	@param i index of bump_map without shifting (specular param)
 *	Shift can occur if the texture is CHECKER
*/
void	initialize_bump_map(t_texture *texture, char **params, int i, int shift)
{
	char		*texture_path;

	if (params[i + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[i + shift]);
		texture->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
}
