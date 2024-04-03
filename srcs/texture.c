#include "miniRT.h"

t_vec3	get_checkered_color(t_vec3 point,
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
	t_vec3	ret;
	int		pixel;

	pixel = texture->pixels[(int)(v * texture->height)
		* (int)(u * texture->width)];
	ret.x = pixel >> 24 & 0xFF;
	ret.y = pixel >> 16 & 0xFF;
	ret.z = pixel >> 8 & 0xFF;
	return (ret);
}

t_vec3	get_bump_map_color(mlx_texture_t *texture, double u, double v)
{
	(void)texture;
	(void)u;
	(void)v;
	return (init_vec3(0, 0, 0));
}
