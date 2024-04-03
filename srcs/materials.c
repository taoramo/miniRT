#include "miniRT.h"

int	lambertian_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered)
{
	t_vec3	scatter_direction;

	(void) r_in;
	scatter_direction = vec3_plus_vec3(rec->normal, random_unit_vector());
	if (near_zero(scatter_direction))
		scatter_direction = rec->normal;
	*scattered = init_ray(rec->point, scatter_direction);
	return (1);
}

int	metal_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered)
{
	t_vec3	reflected;

	reflected = reflect(unit_vector(r_in->direction), rec->normal);
	*scattered = init_ray(rec->point,
			vec3_plus_vec3(reflected,
				vec3_times_d(random_unit_vector(), rec->material1)));
	return (dot(scattered->direction, rec->normal) > 0);
}

int	matte_scatter(t_ray *r_in, t_hit_record *rec, t_ray *scattered)
{
	(void)r_in;
	(void)rec;
	(void)scattered;
	return (1);
}
