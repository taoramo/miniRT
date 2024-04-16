/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_texture.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:25:45 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 16:06:40 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	get_texture_type(char *text_type)
{
	int	str_len;

	str_len = ft_strlen(text_type);
	if (ft_strncmp(text_type, "solid", str_len + 1) == 0)
		return (SOLID);
	else if (ft_strncmp(text_type, "checker", str_len + 1) == 0)
		return (CHECKER);
	else if (ft_strncmp(text_type + str_len - 4, ".png", 5) == 0)
		return (PNG_FILE);
	return (SOLID);
}

/**
 * Shift occurs when a CHECKER type texture is used.
 * Additional color param will be added.
*/
int	get_index_shift(t_texture *texture)
{
	int	shift;

	shift = 0;
	if (texture->type == CHECKER)
		shift = 1;
	return (shift);
}

/**
 * @param i index of texture type in params
*/
void	initialize_texture(t_texture *texture, char **params, int i)
{
	char		*texture_path;

	texture->type = get_texture_type(params[i]);
	if (texture->type == CHECKER)
	{
		initialize_coordinate(&texture->checker_color, params[i + 1]);
		texture->checker_color = vec3_div_d(texture->checker_color, 255.0);
		texture->checkered = 1;
		texture->checker_size_coeff = DEFAULT_CHECKER_SIZE;
	}
	if (texture->type == PNG_FILE)
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[i]);
		texture->texture_obj = mlx_load_png(texture_path);
		free(texture_path);
	}
}
