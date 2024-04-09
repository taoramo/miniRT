#include "miniRT.h"

double	get_pixel_bw(mlx_texture_t *bm, int x, int y)
{
	uint8_t	*pixel;
	t_vec3	ret;

	pixel = bm->pixels + (y * bm->width + x) * 4;
	ret.x = (*(pixel)) / 255.0;
	ret.y = (*(pixel + 1)) / 255.0;
	ret.z = (*(pixel + 2)) / 255.0;
	return ((ret.x + ret.y + ret.z) / 3);
}

t_vec3	bump_map(t_hit_record *rec, mlx_texture_t *bm)
{
	int		x;
	int		y;
	t_vec3	ret;

	x = rec->u * bm->width;
	y = (-1 * rec->v + 1) * bm->height;
	ret = vec3_times_d(rec->u_vector, get_pixel_bw(bm, x, y)
			- get_pixel_bw(bm, (x + 1) % bm->width, y));
	ret = vec3_plus_vec3(ret, vec3_times_d(rec->v_vector, get_pixel_bw(bm, x, y)
				- get_pixel_bw(bm, x, (y + 1) % bm->height)));
	ret = vec3_plus_vec3(ret, rec->normal);
	return (ret);
}
