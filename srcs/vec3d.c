#include "miniRT.h"

t_vec3	random_on_hemisphere(t_vec3	normal)
{
	t_vec3	on_unit_sphere;

	on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
		return (on_unit_sphere);
	else
		return (vec3_times_d(on_unit_sphere, -1.0));
}
