/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 08:56:46 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 08:56:47 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	get_solid_checkered_color(t_vec3 point,
		double coeff, t_vec3 color1, t_vec3 color2)
{
	int	x;
	int	y;
	int	z;

	x = (int)floor(coeff * point.x);
	y = (int)floor(coeff * point.y);
	z = (int)floor(coeff * point.z);
	if ((x + y + z) % 2)
		return (color1);
	else
		return (color2);
}

t_vec3	get_texture_color(mlx_texture_t *texture, double u, double v)
{
	t_vec3		ret;
	int			x;
	int			y;
	uint8_t		*pixelstart;

	if (!texture)
		return (init_vec3(0, 0, 0));
	x = (int)(u * texture->width) % texture->width;
	y = (int)((-1.0 * v + 1) * texture->height) % texture->height;
	pixelstart = texture->pixels
		+ (y * texture->width + x) * 4;
	ret.x = (*(pixelstart)) / 255.0;
	ret.y = (*(pixelstart + 1)) / 255.0;
	ret.z = (*(pixelstart + 2)) / 255.0;
	return (ret);
}
