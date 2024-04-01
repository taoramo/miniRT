#include "miniRT.h"

int	hit_sphere(t_ray ray, double ray_tmin, double ray_tmax, t_hit_record rec, t_sphere sphere)
{
	t_vec3	oc;
	double	a;
	double	half_b;
	double	c;
	double	discriminant;
	double	root;

	oc = vec3_minus_vec3(ray.origin, sphere.origin);
	a = vec3length_squared(ray.direction);
	half_b = dot(oc, ray.direction);
	c = vec3length_squared(oc) - sphere.radius * sphere.radius;
	discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return (0);
	root = (-1.0 * half_b - sqrt(discriminant)) / a;
	if (root <= ray_tmin || ray_tmax <= root)
	{
		root = (-1.0 * half_b + sqrt(discriminant)) / a;
		if (root <= ray_tmin || ray_tmax <= root)
			return (0);
	}
	rec.t = root;
	rec.point = ray_at(ray, root);
	rec.normal = vec3_div_d(vec3_minus_vec3(rec.point,
				sphere.origin), sphere.radius);
	set_face_normal(&rec, ray, sphere);
	return (1);
}
