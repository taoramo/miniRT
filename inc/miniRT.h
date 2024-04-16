#ifndef MINIRT_H
# define MINIRT_H

# include "MLX42.h"
# include "libft.h"
# include "vec3.h"
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
// TODO: remove this header
# include <stdio.h>
# include "libft.h"
# define WWIDTH 1280
# define WHEIGHT 720
# define N_MATERIALS 3
# define N_OBJECT_TYPES 7
# define EPSILON 1e-8
# define BUMP_SCALE 2

# define EMPTY_LINE 2
# define SAMPLES_PER_PIXEL 10
# define MAX_DEPTH 4
# define DEFAULT_CHECKER_SIZE 20
# define TEXTURES_PATH "./"

typedef struct s_pixel
{
	int		x;
	int		y;
	t_vec3	color;
}	t_pixel;

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
	t_vec3	background_color;
	t_vec3	ambient_color;
	t_vec3	ambient_ratio;
}	t_camera;

typedef struct s_ambient
{
	double	brightness;
	t_vec3	color;
}	t_ambient;

typedef struct s_light
{
	t_vec3	point;
	t_vec3	color;
}	t_light;


typedef enum e_texture_type
{
	SOLID,
	CHECKER,
	PNG_FILE,
}	t_texture_type;

typedef	struct s_texture
{
	int				checkered;
	t_vec3			checker_color;
	double			checker_size_coeff;
	t_texture_type	type;
	mlx_texture_t	*texture_obj;
	mlx_texture_t	*bump_map;
}	t_texture;

typedef struct s_phong
{
	double			k_s;
	double			k_d;
	double			material1;
	t_vec3			emitted;
}	t_phong;

typedef struct s_sphere
{
	t_vec3			origin;
	double			radius;
	t_vec3			albedo;
	t_texture		texture;
	t_phong			phong;
}	t_sphere;

typedef struct s_plane
{
	t_vec3			point;
	t_vec3			normal;
	t_vec3			albedo;
	t_texture		texture;
	t_phong			phong;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3			center;
	t_vec3			axisnormal;
	double			radius;
	double			height;
	t_vec3			albedo;
	t_texture		texture;
	t_phong			phong;
	double			m;
	t_vec3			oc;
}	t_cylinder;

typedef struct s_cone
{
	double			angle;
	double			height;
	t_vec3			tip;
	t_vec3			axis;
	t_vec3			albedo;
	t_texture		texture;
	t_phong			phong;
}	t_cone;

typedef struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef enum s_object_type
{
	A,
	C,
	l,
	sp,
	pl,
	cy,
	co
}	t_object_type;

typedef struct s_master
{
	mlx_t			*mlx;
	int				objects_count[N_OBJECT_TYPES]; // A C l sp pl cy co
	const char		**ids;
	t_camera		camera;
	int				samples_per_pixel;
	int				max_depth;
	t_ambient		ambient;
	t_light			*lights;
	unsigned int	n_lights;
	t_sphere		*spheres;
	unsigned int	n_spheres;
	t_plane			*planes;
	unsigned int	n_planes;
	t_cylinder		*cylinders;
	unsigned int	n_cylinders;
	t_cone			*cones;
	unsigned int	n_cones;
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
	t_vec3		albedo;
	double		material1;
	double		u;
	double		v;
	t_vec3		emitted;
	double		k_d;
	double		k_s;
	int			is_temp;
	t_vec3		u_vector;
	t_vec3		v_vector;
}	t_hit_record;

typedef struct s_loop
{
	int				hit_anything;
	double			closest_so_far;
	t_interval		t_minmax;
}	t_loop;

typedef struct s_hit_cylinder
{
	double		root1;
	double		root2;
	t_ray		*ray;
	t_cylinder	*cylinder;
	t_interval	t_minmax;
	t_vec3		oc;
	double		a;
	double		half_b;
	double		c;
	double		discriminant;
}	t_hit_cylinder;

typedef struct s_hit_cone
{
	double		root1;
	double		root2;
	t_ray		*ray;
	t_cone		*cone;
	t_interval	t_minmax;
	t_vec3		oc;
	double		angle;
	double		a;
	double		half_b;
	double		c;
	double		discriminant;
}	t_hit_cone;

typedef struct s_ray_colors
{
	t_vec3			diffuse;
	t_vec3			specular;
	t_vec3			shadow;
	t_vec3			emitted;
}	t_ray_colors;

// typedef int	(*t_f) (t_ray *r_in, t_hit_record *rec, t_ray *scattered); // is it used anywhere?

typedef int	(*t_val_f) (char *str);

/* Utilities */
typedef int	(*t_validate_str)(char *value_param);
double			ft_atod(const char *str);
bool			is_space(char c);
bool			is_capital(char c);
void			free_split(char **split);
int				index_of(char **arr, char *str);
int				str_array_length(char **str);
void			free_all(t_master *m);

/* Validator */
void			replace_whitespaces(char *line);

int				validate_int_str(char *int_str);
int				validate_f_str(char *f_str);
int				validate_f_range(char *f_str, float min, float max, char *err);
int				validate_three_tuple_size(char *value_param);

int				print_error(char *err);

int				validate_position(char *value_param);
int				validate_orientation(char *value_param);
int				validate_size(char *value_param);
int				validate_rgb(char *value_param);
int				validate_texture(char *value_param);
int				checker_shift(char **value_params);
int				validate_checker_rgbs(char **value_params);
int				validate_0_to_1(char *value_param);
int				validate_emitted(char *value_param);
int				validate_bump_map(char *value_param);

int				validate_param(char *value_param, t_val_f f,
					t_interval range, char *err);

int				validate_ambient_light(char **value_params);
int				validate_camera(char **value_params);
int				validate_light(char **value_params);
int				validate_sphere(char **value_params);
int				validate_plane(char **value_params);
int				validate_cylinder(char **value_params);
int				validate_cone(char **value_params);

int				validate_line_identifier(char *line, int objects_count[],
					const char *ids[]);

int				prepare_line(char **line, int fd);
int				validate(const char *argv[],
					int objects_count[], const char *ids[]);

int				allocate_objects(int objects_count[], t_master *m);

/* Initializer */
void			initialize_master_struct(t_master *m, const char *ids[]);
int				initialize(t_master *m, mlx_t **mlx, const char *argv[]);

void			initialize_ambient(t_master *m, char **params);
void			initialize_camera(t_master *m, char **params);
void			initialize_light(t_master *m, char **params);

void			initialize_sphere(t_master *m, char **params);
void			initialize_plane(t_master *m, char **params);
void			initialize_cylinder(t_master *m, char **params);
void			initialize_cone(t_master *m, char **params);

void			initialize_coordinate(t_vec3 *coord, char *value_param);
void			initialize_texture(t_texture *texture, char **params, int i);
void			initialize_phong(t_phong *phong, char **params, int i,
					int shift);
void			initialize_bump_map(t_texture *texture, char **params, int i,
					int shift);

int				get_index_shift(t_texture *texture);

/* Ray tracer */
int				render(t_master *m, mlx_t *mlx);
t_ray			init_ray(t_vec3 origin, t_vec3 direction);
unsigned int	colorsum_to_rgba(t_color c, int samples_per_pixel);
t_color			ray_color(t_master *m, t_ray *r, int max_depth);
t_vec3			ray_at(t_ray r, double t);
int				hit(t_master *m, t_ray *r,
					t_interval t_minmax, t_hit_record *rec);
int				hit_sphere(t_ray *ray, t_interval t_minmax,
					double *t, t_sphere *sphere);
void			set_sphere_rec(t_hit_record *rec,
					t_sphere *sphere, t_ray *ray, double t);
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
int				lambertian_scatter(t_hit_record *rec, t_ray *scattered);
int				near_zero(t_vec3 vec);
t_vec3			reflect(t_vec3 v, t_vec3 n);
int				metal_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered);
t_vec3			get_checkered_color(t_vec3 point, double coeff,
					t_vec3 color1, t_vec3 color2);
t_vec3			get_texture_color(mlx_texture_t *texture, double u, double v);
int				hit_plane(t_ray *ray, t_interval t_minmax,
					double *t_new, t_plane *plane);
void			set_plane_rec(t_hit_record *rec,
					t_plane *plane, t_ray *ray, double t);
int				hit_cylinder(t_ray *ray, t_interval t_minmax,
					double *t, t_cylinder *cylinder);
void			set_cylinder_rec(t_hit_record *rec,
					t_cylinder *cylinder, t_ray *ray, double t);

t_vec3			get_solid_checkered_color(t_vec3 point,
					double coeff, t_vec3 color1, t_vec3 color2);
t_vec3			bump_map(t_hit_record *rec, mlx_texture_t *bm);

int				hit_cone(t_ray *ray, t_interval t_minmax,
					double *t, t_cone *cone);
void			set_cone_rec(t_hit_record *rec,
					t_cone *cone, t_ray *ray, double t);

#endif
