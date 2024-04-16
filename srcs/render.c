#include "miniRT.h"

static void	calculate_camera(t_camera *c)
{
	double	viewport_width;
	double	viewport_height;
	t_vec3	viewport_upper_left;
	t_vec3	w;
	t_vec3	u;

	viewport_width = tan(degrees_to_radians(c->hfov / 2))
		* 2 * c->focal_length;
	viewport_height = viewport_width * (WHEIGHT * 1.0) / (WWIDTH * 1.0);
	w = unit_vector(vec3_minus_vec3(c->camera_center, c->look_at));
	u = unit_vector(cross(init_vec3(0, 1, 0), w));
	c->viewport_u = vec3_times_d(u, viewport_width);
	c->viewport_v = vec3_times_d(cross(w, u), viewport_height * -1.0);
	c->pixel_delta_u = vec3_div_d(c->viewport_u, WWIDTH * 1.0);
	c->pixel_delta_v = vec3_div_d(c->viewport_v, WHEIGHT * 1.0);
	viewport_upper_left = vec3_minus_vec3(c->camera_center,
			vec3_times_d(w, c->focal_length));
	viewport_upper_left = vec3_minus_vec3(viewport_upper_left,
			vec3_div_d(c->viewport_u, 2));
	viewport_upper_left = vec3_minus_vec3(viewport_upper_left,
			vec3_div_d(c->viewport_v, 2));
	c->pixel00_loc = vec3_plus_vec3(viewport_upper_left,
			vec3_times_d(vec3_plus_vec3(c->pixel_delta_v,
					c->pixel_delta_u), 0.5));
}

static void	make_pixel(t_master *m, t_pixel *pixel, t_ray *ray)
{
	int		sample;

	sample = -1;
	pixel->color = init_vec3(0, 0, 0);
	while (++sample < m->samples_per_pixel)
	{
		*ray = get_ray(&m->camera, pixel->x, pixel->y);
		pixel->color = vec3_plus_vec3(pixel->color,
				ray_color(m, ray, m->max_depth));
	}
}

static void	make_image(t_master *m, mlx_image_t *img)
{
	t_pixel		pixel;
	t_ray		ray;

	calculate_camera(&m->camera);
	pixel.color = init_vec3(-1, -1, 0);
	pixel.y = -1;
	while (++pixel.y < WHEIGHT)
	{
		pixel.x = -1;
		while (++pixel.x < WWIDTH)
		{
			make_pixel(m, &pixel, &ray);
			mlx_put_pixel(img, pixel.x, pixel.y, colorsum_to_rgba(pixel.color,
					m->samples_per_pixel));
		}
		if (pixel.y % 100 == 0)
			write(1, "x", 1);
	}
}

int	render(t_master *m, mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WWIDTH, WHEIGHT);
	if (!img)
		return (print_error("New image failed."));
	make_image(m, img);
	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (print_error("Image to window failed."));
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
