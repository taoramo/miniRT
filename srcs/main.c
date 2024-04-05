#include "MLX42.h"
#include "miniRT.h"
#include "vec3.h"

int	ft_error(void)
{
	return (1);
}
// TODO: calculate focal_length from FOV angle

void	calculate_camera(t_camera *c)
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

void	make_image(t_master *m, mlx_image_t *img)
{
	t_vec3		color;
	int			i;
	int			j;
	int			sample;
	t_ray		ray;

	calculate_camera(m->camera);
	j = 0;
	while (j < WHEIGHT)
	{
		i = 0;
		while (i < WWIDTH)
		{
			sample = 0;
			color = init_vec3(0, 0, 0);
			while (sample < m->samples_per_pixel)
			{
				ray = get_ray(m->camera, i, j);
				color = vec3_plus_vec3(color,
						ray_color(m, &ray, m->max_depth));
				sample++;
			}
			mlx_put_pixel(img, i, j, colorsum_to_rgba(color,
					m->samples_per_pixel));
			i++;
		}
		j++;
		if (j % 100 == 0)
			write(1, "x", 1);
	}
	return ;
}

int	render(t_master *m, mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WWIDTH, WHEIGHT);
	if (!img)
		return (ft_error());
	make_image(m, img);
	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (ft_error());
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}

int	main(void)
{
	t_master	m;
	t_camera	camera;
	mlx_t		*mlx;

	mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
	if (!mlx)
		return (ft_error());
	m.camera = &camera;
	m.camera->hfov = 100;
	m.camera->focal_length = 1.0;
	m.camera->camera_center = init_vec3(0, 4, 10);
	m.camera->look_at = init_vec3(0, 0, -1);
	m.samples_per_pixel = 10;
	m.max_depth = 5;
	m.n_spheres = 2;
	m.spheres = malloc(sizeof(t_sphere) * m.n_spheres);
	m.spheres[0].origin = init_vec3(1, 0, -1);
	m.spheres[0].radius = 0.5;
	m.spheres[0].material = lambertian;
	m.spheres[0].material1 = 0;
	m.spheres[0].texture_type = texture;
	m.spheres[0].texture = mlx_texture_to_image(mlx, mlx_load_png("./earthmap.png"));
	m.spheres[0].albedo = init_vec3(0.8, 0.3, 0.3);
	m.spheres[0].checkered = 1;
	m.spheres[0].checker_color = init_vec3(1, 0, 0);
	m.spheres[1].origin = init_vec3(-1, 0, -1);
	m.spheres[1].radius = 0.5;
	m.spheres[1].material = metal;
	m.spheres[1].texture_type = solid;
	m.spheres[1].albedo = init_vec3(1.0, 0, 0);
	m.spheres[1].material1 = 0.3;
	m.spheres[1].checkered = 0;
	m.n_planes = 1;
	m.cylinders = malloc(sizeof(t_plane) * m.n_planes);
	m.cylinders[0].center = init_vec3(0, 3, 0);
	m.cylinders[0].axisnormal = init_vec3(0, 1, 0);
	m.cylinders[0].height = 2;
	m.cylinders[0].radius = 1;
	m.cylinders[0].albedo = init_vec3(1, 0.2, 0.2);
	m.cylinders[0].material = lambertian;
	m.cylinders[0].texture_type = solid;
	render(&m, mlx);
	free(m.spheres);
	free(m.cylinders);
	return (0);
}
