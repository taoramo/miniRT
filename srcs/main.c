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

	calculate_camera(&m->camera);
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
				ray = get_ray(&m->camera, i, j);
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
		return (print_error("New image failed."));
	make_image(m, img);
	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (print_error("Image to window failed."));
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}

int	main(int argc, char const *argv[])
{
	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "l", "sp", "pl", "cy", "co"};
	t_master	m;
	mlx_t		*mlx;

	// Init master struct
	mlx = NULL;
	initialize_master_struct(&m, ids);
	ft_bzero(m.objects_count, sizeof(int) * N_OBJECT_TYPES);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	if (validate(argv, m.objects_count, ids) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_objects(m.objects_count, &m) == EXIT_FAILURE)
		return (EXIT_FAILURE);
/* 	//Test object count
	int i = 0;
	while (i < N_OBJECT_TYPES)
	{
		printf("%s : %d\n", m.ids[i], m.objects_count[i]); // leads to Conditional jump or move depends on uninitialised value(s) and Uninitialised value was created by a stack allocation
		i++;
	} */
	initialize(&m, &mlx, argv);
	if (render(&m, mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free(m.spheres);
	free(m.planes);
	free(m.cylinders);
	free(m.lights);
	free(m.cones);
	return (EXIT_SUCCESS);
}
