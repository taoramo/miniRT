#include "miniRT.h"

t_vec3	bumps(mlx_texture_t *bumps, t_hit_record *rec)
{
	int		x;
	int		y;
	t_vec3	uplus;
	t_vec3	vplus;
	uint8_t	*pixel;
	
	x = u * bumps->width;
	y = (-1.0 * v + 1) * bumps->height;
	pixel = bumps->pixels + (y * bumps->width + x) * 4;
	uplus = 	
