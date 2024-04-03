#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42.h"
# include "vec3.h"
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
// TODO: remove this header
# include <stdio.h>
# define WWIDTH 1280
# define WHEIGHT 720
# define N_MATERIALS 3

typedef struct s_camera
{
	double	hfov;
	double	focal_length;
	double	viewport_height;
	double	viewport_width;
	t_vec3	camera_center;
	t_vec3	look_at;
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

typedef enum e_type
{
	lambertian,
	metal,
	phong
}	t_type;

typedef enum e_texture_type
{
	solid,
	checker,
	texture,
	bump_map
}	t_texture_type;

typedef struct s_sphere
{
	t_vec3			origin;
	double			radius;
	t_type			material;
	t_vec3			albedo;
	double			material1;
	int				checkered;
	t_vec3			checker_color;
	double			checker_size_coeff;
	t_texture_type	texture_type;
	mlx_texture_t	*texture;
}	t_sphere;

typedef struct s_plane
{
	t_vec3			point;
	t_vec3			normal;
	t_vec3			color;
	t_type			material;
	t_vec3			albedo;
	double			material1;
	int				checkered;
	t_vec3			checker_color;
	double			checker_size_coeff;
	t_texture_type	texture_type;
	mlx_texture_t	*texture;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3		center;
	t_vec3		axisnormal;
	double		radius;
	double		height;
	t_vec3		color;
	t_type		material;
	t_vec3		albedo;
	double		material1;
}	t_cylinder;

typedef struct s_cone
{
	double		angle;
	double		height;
	t_vec3		tip;
	t_vec3		axis;
	t_type		material;
	t_vec3		albedo;
	double		material1;
}	t_cone;

typedef struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef struct s_master
{
	t_camera		*camera;
	int				samples_per_pixel;
	int				max_depth;
	t_ambient		ambient;
	t_light			*lights;
	unsigned int	n_lights;
	t_sphere		*spheres;
	unsigned int	n_spheres;
	t_plane			*planes;
	unsigned int	n_planes;
	t_cone			*cones;
	unsigned int	n_cones;
	t_cylinder		*cylinders;
	unsigned int	n_cylinders;
}	t_master;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_hit_record
{
	t_vec3		point;
	t_vec3		normal;
	double		t;
	int			front_face;
	t_type		material;
	t_vec3		albedo;
	double		material1;
	double		u;
	double		v;
}	t_hit_record;

typedef int	(*t_f) (t_ray *r_in, t_hit_record *rec, t_ray *scattered);

t_ray			init_ray(t_vec3 origin, t_vec3 direction);
unsigned int	colorsum_to_rgba(t_color c, int samples_per_pixel);
t_color			ray_color(t_master *m, t_ray *r, int max_depth);
t_vec3			ray_at(t_ray r, double t);

int				hit_sphere(t_ray *ray, t_interval t_minmax,
					t_hit_record *rec, t_sphere *sphere);
void			set_face_normal(t_hit_record *rec, t_ray *r,
					t_sphere *sphere);
t_interval		init_interval(double min, double max);
int				contains(t_interval i, double x);
int				surrounds(t_interval i, double x);
double			clamp(t_interval i, double x);
double			degrees_to_radians(double degrees);
double			random_double(void);
double			random_double_between(double min, double max);
t_ray			get_ray(t_camera *c, int i, int j);
t_vec3			pixel_sample_square(t_camera *c);
t_vec3			random_vec3(void);
t_vec3			random_vec3_between(double min, double max);
t_vec3			random_unit_vector(void);
t_vec3			random_on_hemisphere(t_vec3 normal);
double			linear_to_gamma(double linear);
int				lambertian_scatter(t_ray *r_in, t_hit_record *rec,
					t_ray *scattered);
int				near_zero(t_vec3 vec);
t_vec3			reflect(t_vec3 v, t_vec3 n);
int				metal_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered);
int				matte_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered);
t_vec3			get_checkered_color(t_vec3 point, double coeff,
					t_vec3 color1, t_vec3 color2);
t_vec3			get_bump_map_color(mlx_texture_t *texture, double u, double v);
t_vec3			get_texture_color(mlx_texture_t *texture, double u, double v);
#endif
