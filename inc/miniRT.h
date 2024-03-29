#ifndef MINIRT_H
#define MINIRT_H

#include "MLX42/include/MLX42/MLX42.h"
#include "vec3.h"
#define WWIDTH 1280
#define WHEIGHT 720

typedef struct s_master
{
} t_master;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
} t_ray;

typedef struct	s_camera
{
	double	focal_length;
	double	viewport_height;
	double	viewport_width;
	t_vec3	camera_center;
	t_vec3	viewport_u;
	t_vec3	viewport_v;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	t_vec3	pixel00_loc;
} t_camera;

int	cast_ray(int x, int y, t_master m);
t_vec3	ray_at(t_ray r, double t);

#endif
