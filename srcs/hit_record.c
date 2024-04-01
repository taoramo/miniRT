#include "miniRT.h"

void	set_face_normal(t_hit_record *rec, const t_ray r, const t_sphere sphere)
{
	t_vec3	outward_normal;

	outward_normal = vec3_div_d(vec3_minus_vec3(rec->point, sphere.origin), sphere.radius);
	rec->front_face = dot(r.direction, outward_normal) < 0;
	if (rec->front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_times_d(outward_normal, -1.0);
}
