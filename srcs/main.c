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

// int	main(void)
// {
// 	t_master	m;
// 	t_camera	camera;
// 	mlx_t		*mlx;

// 	mlx = mlx_init(WWIDTH, WHEIGHT, "miniRT", true);
// 	if (!mlx)
// 		return (ft_error());
// 	ft_bzero(&m, sizeof(m));
// 	ft_bzero(&camera, sizeof(camera));
// 	m.camera = camera;
// 	m.camera.hfov = 120;
// 	m.camera.focal_length = 1.0;
// 	m.camera.camera_center = init_vec3(0, 2, 2);
// 	m.camera.look_at = init_vec3(0, 0, -1);
// 	m.camera.background_color = init_vec3(0, 0, 0);
// 	m.samples_per_pixel = 10; // 300
// 	m.max_depth = 4;
// 	m.n_spheres = 2;
// 	m.spheres = malloc(sizeof(t_sphere) * m.n_spheres);
// 	m.spheres[0].origin = init_vec3(0, 0, -1);
// 	m.spheres[0].radius = 0.5;
// 	m.spheres[0].material = metal;
// 	m.spheres[0].material1 = 0.3;
// 	m.spheres[0].texture_type = solid;
// 	m.spheres[0].texture = mlx_texture_to_image(mlx, mlx_load_png("./earthmap.png"));
// 	m.spheres[0].albedo = init_vec3(0.8, 0.1, 0.1);
// 	m.spheres[0].checkered = 1;
// 	m.spheres[0].checker_color = init_vec3(1, 0, 0);
// 	m.spheres[0].emitted = init_vec3(0, 0, 0);
// 	m.spheres[0].k_d = 0.5;
// 	m.spheres[0].k_s = 1;
// 	m.spheres[1].emitted = init_vec3(10, 10, 10);
// 	m.spheres[1].origin = init_vec3(-1, 1, -1);
// 	m.spheres[1].radius = 0.25;
// 	m.spheres[1].material = lambertian;
// 	m.spheres[1].texture_type = solid;
// 	m.spheres[1].albedo = init_vec3(0.5, 0.5, 0.5);
// 	m.spheres[1].material1 = 0;
// 	m.spheres[1].checker_color = init_vec3(1, 1, 1);
// 	m.spheres[1].checker_size_coeff = 8;
// 	m.spheres[1].k_s = 1;
// 	m.spheres[1].k_d = 0.5;
// 	m.n_planes = 1;
// 	m.planes = ft_calloc(sizeof(t_plane), m.n_planes);
// 	m.planes[0].point = init_vec3(0, -1, 0);
// 	m.planes[0].normal = init_vec3(0, 1, 0);
// 	m.planes[0].material = lambertian;
// 	m.planes[0].albedo = init_vec3(0.5, 0.5, 0.8);
// 	m.planes[0].texture_type = solid;
// 	m.planes[0].checker_color = init_vec3(1, 1, 1);
// 	m.planes[0].checker_size_coeff = 3;
// 	m.planes[0].texture = mlx_texture_to_image(mlx, mlx_load_png("./earthmap.png"));
// 	m.planes[0].emitted = init_vec3(0, 0, 0);
// 	m.planes[0].k_d = 1;
// 	m.planes[0].k_s = 0.5;
// 	m.planes[0].material1 = 0;
// 	m.n_cylinders = 1;
// 	m.cylinders = malloc(sizeof(t_cylinder) * m.n_cylinders);
// 	m.cylinders[0].center = init_vec3(1, 1, -1);
// 	m.cylinders[0].axisnormal = unit_vector(init_vec3(0, 1, 0));
// 	m.cylinders[0].height = 1;
// 	m.cylinders[0].radius = 0.5;
// 	m.cylinders[0].albedo = init_vec3(0.5, 0.3, 0.8);
// 	m.cylinders[0].material = metal;
// 	m.cylinders[0].material1 = 0.1;
// 	m.cylinders[0].texture_type = solid;
// 	m.cylinders[0].checker_size_coeff = 2;
// 	m.cylinders[0].checker_color = init_vec3(1, 0, 0);
// 	m.cylinders[0].emitted = init_vec3(0, 0, 0);
// 	m.cylinders[0].k_s = 0.8;
// 	m.cylinders[0].k_d = 0.6;
// 	render(&m, mlx);
// 	free(m.spheres);
// 	free(m.planes);
// 	free(m.cylinders);
// 	return (0);
// }


/* void close_hook(void *master)
{
	t_master *m;
	
	printf("Close hook was called!\n");
	m = (t_master *)master;
	free(m->spheres);
	free(m->planes);
	free(m->cylinders);
	free(m->lights);
	free(m->cones);
} */

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
	//Test object count
	int i = 0;
	while (i < N_OBJECT_TYPES)
	{
		printf("%s : %d\n", m.ids[i], m.objects_count[i]); // leads to Conditional jump or move depends on uninitialised value(s) and Uninitialised value was created by a stack allocation
		i++;
	}
	initialize(&m, &mlx, argv);

	// mlx_close_hook(mlx, &close_hook, &m);
	if (render(&m, mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free(m.spheres);
	free(m.planes);
	free(m.cylinders);
	free(m.lights);
	free(m.cones);
	return (EXIT_SUCCESS);
}
