#include "miniRT.h"

int	lambertian_scatter(t_hit_record rec, t_ray *scattered)
{
	t_vec3	scatter_direction;

	scatter_direction = vec3_plus_vec3(rec.normal, random_unit_vector());
	if (near_zero(scatter_direction))
		scatter_direction = rec.normal;
	*scattered = init_ray(rec.point, scatter_direction);
	return (1);
}
