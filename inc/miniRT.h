#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42.h"
# include "vec3.h"
# include <math.h>
// TODO: remove this header
# include <stdio.h>
# define WWIDTH 1280
# define WHEIGHT 720

typedef struct s_camera
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
}	t_camera;

typedef struct s_ambient
{
	double	brightness;
	t_vec3	color;
}	t_ambient;

typedef struct s_light
{
	t_vec3	point;
	t_vec3	orientation;
	t_vec3	color;
	t_vec3	brightness;
}	t_light;

typedef struct s_sphere
{
	unsigned int	object_index;
	t_vec3			origin;
	double			radius;
	t_vec3			color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
	t_vec3	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axisnormal;
	double	radius;
	double	height;
	t_vec3	color;
}	t_cylinder;

typedef struct s_cone
{
	double	angle;
	double	height;
	t_vec3	tip;
	t_vec3	axis;
}	t_cone;

typedef struct s_master
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cone		*cones;
	t_cylinder	*cylinders;
}	t_master;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

t_ray			init_ray(t_vec3 origin, t_vec3 direction);
unsigned int	color_to_rgba(t_color c);
t_color			ray_color(t_master *m, t_ray r);
t_vec3			ray_at(t_ray r, double t);

bool			hit_sphere(t_vec3 center, double radius, t_ray ray);

#endif
