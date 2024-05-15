/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 08:50:42 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 08:55:31 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	loop_through_spheres(t_loop *loop, t_ray *r,
						t_master *m, t_hit_record *rec)
{
	unsigned int	i;
	double			t;

	i = 0;
	while (i < m->n_spheres)
	{
		if (hit_sphere(r, init_interval(loop->t_minmax.min,
					loop->closest_so_far), &t, &m->spheres[i]))
		{
			loop->hit_anything = 1;
			loop->closest_so_far = t;
			if (rec)
				set_sphere_rec(rec, &m->spheres[i], r, t);
		}
		i++;
	}
}

void	loop_through_planes(t_loop *loop, t_ray *r,
						t_master *m, t_hit_record *rec)
{
	unsigned int	i;
	double			t;

	i = 0;
	while (i < m->n_planes)
	{
		if (hit_plane(r, init_interval(loop->t_minmax.min,
					loop->closest_so_far), &t, &m->planes[i]))
		{
			loop->hit_anything = 1;
			loop->closest_so_far = t;
			if (rec)
				set_plane_rec(rec, &m->planes[i], r, t);
		}
		i++;
	}
}

void	loop_through_cylinders(t_loop *loop, t_ray *r,
						t_master *m, t_hit_record *rec)
{
	unsigned int	i;
	double			t;

	i = 0;
	while (i < m->n_cylinders)
	{
		if (hit_cylinder(r, init_interval(loop->t_minmax.min,
					loop->closest_so_far), &t, &m->cylinders[i]))
		{
			loop->hit_anything = 1;
			loop->closest_so_far = t;
			if (rec)
				set_cylinder_rec(rec, &m->cylinders[i], r, t);
		}
		i++;
	}
}

void	loop_through_cones(t_loop *loop, t_ray *r,
						t_master *m, t_hit_record *rec)
{
	unsigned int	i;
	double			t;

	i = 0;
	while (i < m->n_cones)
	{
		if (hit_cone(r, init_interval(loop->t_minmax.min,
					loop->closest_so_far), &t, &m->cones[i]))
		{
			loop->hit_anything = 1;
			loop->closest_so_far = t;
			if (rec)
				set_cone_rec(rec, &m->cones[i], r, t);
		}
		i++;
	}
}

int	hit(t_master *m, t_ray *r, t_interval t_minmax, t_hit_record *rec)
{
	t_loop	loop;

	ft_bzero(&loop, sizeof(loop));
	loop.t_minmax = t_minmax;
	loop.closest_so_far = t_minmax.max;
	loop_through_spheres(&loop, r, m, rec);
	loop_through_planes(&loop, r, m, rec);
	loop_through_cylinders(&loop, r, m, rec);
	loop_through_cones(&loop, r, m, rec);
	return (loop.hit_anything);
}
