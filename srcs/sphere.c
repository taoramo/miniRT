#include "miniRT.h"

int	hit_sphere(t_ray *ray, t_interval t_minmax,
			t_hit_record *rec, t_sphere sphere)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	discriminant;
	double	root;

	oc = vec3_minus_vec3(ray->origin, sphere.origin);
	a = vec3length_squared(ray->direction);
	half_b = dot(oc, ray->direction);
	discriminant = half_b * half_b - a
		* (vec3length_squared(oc) - sphere.radius * sphere.radius);
	if (discriminant < 0)
		return (0);
	root = (-1.0 * half_b - sqrt(discriminant)) / a;
	if (!surrounds(t_minmax, root))
	{
		root = (-1.0 * half_b + sqrt(discriminant)) / a;
		if (!surrounds(t_minmax, root))
			return (0);
	}
	rec->t = root;
	rec->point = ray_at(*ray, root);
	rec->material = sphere.material;
	rec->albedo = sphere.albedo;
//	printf("%f %f %f \n", sphere.albedo.x, sphere.albedo.y, sphere.albedo.z);
	set_face_normal(rec, ray, sphere);
	return (1);
}

void	set_face_normal(t_hit_record *rec, t_ray *r, t_sphere sphere)
{
	t_vec3	outward_normal;

	outward_normal = vec3_div_d(vec3_minus_vec3(rec->point,
				sphere.origin), sphere.radius);
	rec->front_face = dot(r->direction, outward_normal) < 0;
	if (rec->front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec3_times_d(outward_normal, -1.0);
}
