#include "miniRT.h"
#include "vec3.h"

int	ft_error(void)
{
	return (1);
}

void	calculate_camera(t_camera *c, double focal_length, double viewport_height)
{
	double viewport_width;
	t_vec3 viewport_upper_left;

	focal_length = 2.0;
	viewport_height = 2.0;
	viewport_width = viewport_height * WWIDTH / WHEIGHT;
	c->camera_center = init_vec3(0, 0, 0);
	c->viewport_u = init_vec3(viewport_width, 0, 0);
	c->viewport_v = init_vec3(0, viewport_height * -1.0, 0);
	c->pixel_delta_u = vec3_div_d(c->viewport_u, WWIDTH * 1.0);
	c->pixel_delta_v = vec3_div_d(c->viewport_v, WHEIGHT * 1.0);
	viewport_upper_left = vec3_minus_vec3(c->camera_center, init_vec3(0, 0, c->focal_length));
	viewport_upper_left = vec3_minus_vec3(viewport_upper_left, vec3_times_d(c->viewport_v, 0.5));
	viewport_upper_left = vec3_minus_vec3(viewport_upper_left, vec3_times_d(c->viewport_u, 0.5));
	c->pixel00_loc = vec3_plus_vec3(viewport_upper_left, vec3_times_d(vec3_plus_vec3(c->pixel_delta_v, c->pixel_delta_u), 0.5));
}

int	cast_ray(int x, int y, t_master m)
{
	t_camera	c;
	
	calculate_camera(&c, 1.0, 2.0);
}

int	render(t_master m)
{
	mlx_t*			mlx;
	mlx_image_t*	img;
	int				i;
	int				j;

	mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
	if (!mlx)
		return (ft_error());
	img = mlx_new_image(mlx, WWIDTH, WHEIGHT);
	if (!img)
		return (ft_error());
	i = 0;
	j = 0;
	while (i < WHEIGHT)
	{
		while (j < WWIDTH)
		{
			mlx_put_pixel(img, j, i, cast_ray(j, i, m));
			j++;
		}
		i++;
	}
	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (ft_error());
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
