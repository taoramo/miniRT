#ifndef MINIRT_H
#define MINIRT_H

#include "MLX42.h"
#include "vec3.h"
// TODO: remove this header
#include <stdio.h>
#define WWIDTH 1280
#define WHEIGHT 720

typedef struct s_master
{
	int	x;
} t_master;

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

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
} t_ray;

t_ray	init_ray(t_vec3 origin, t_vec3 direction);
unsigned int	color_to_rgba(t_color c);
t_color	ray_color(t_master *m, t_ray r);
t_vec3	ray_at(t_ray r, double t);

#endif
