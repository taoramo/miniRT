#include "miniRT.h"
#include <stdint.h>

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

	x = u * texture->width;
	y = (-1.0 * v + 1) * texture->height;
	pixelstart = texture->pixels
		+ (y * texture->width + x) * 4;
	ret.x = (*(pixelstart)) / 255.0;
	ret.y = (*(pixelstart + 1)) / 255.0;
	ret.z = (*(pixelstart + 2)) / 255.0;
	return (ret);
}
