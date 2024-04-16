/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:18:37 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/16 15:07:32 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 *	@param i index of k_s without shifting (specular param) 
*/
void	initialize_phong(t_phong *phong, char **params, int i, int shift)
{
	// k_s
	phong->k_s = ft_atod(params[i++ + shift]);
	// k_d
	phong->k_d = ft_atod(params[i++ + shift]);
	// fuzz
	phong->material1 = ft_atod(params[i++ + shift]);
	// Emission
	initialize_coordinate(&phong->emitted, params[i++ + shift]);
}

/**
 *	@param i index of bump_map without shifting (specular param)
 *	Shift can occur if the texture is CHECKER
*/
void	initialize_bump_map(t_texture *texture, char **params, int i, int shift)
{
	char		*texture_path;

	if (params[i + shift])
	{
		texture_path = ft_strjoin(TEXTURES_PATH, params[i + shift]);
		texture->bump_map = mlx_load_png(texture_path);
		free(texture_path);
	}
}

// Initialize Sphere
void	initialize_sphere(t_master *m, char **params)
{
	int			i;
	int			j;
	t_sphere	*sphere;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	sphere = &((m->spheres)[j]);
	sphere->texture.bump_map = NULL;
	sphere->texture.texture_obj = NULL;
	sphere->texture.checkered = 0;
	initialize_coordinate(&sphere->origin, params[1]);
	sphere->radius = ft_atod(params[2]) / 2;
	initialize_coordinate(&sphere->albedo, params[3]);
	sphere->albedo = vec3_div_d(sphere->albedo, 255.0);
	// Texture
	initialize_texture(&sphere->texture, params, 4);
	shift = get_index_shift(&sphere->texture);
	// Phong
	initialize_phong(&sphere->phong, params, 5, shift);
	// Bump map
	initialize_bump_map(&sphere->texture, params, 9, shift);
	(m->objects_count)[i] -= 1;
}

// Initialize Plane
void	initialize_plane(t_master *m, char **params)
{
	int			i;
	int			j;
	t_plane		*plane;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	plane = &((m->planes)[j]);
	plane->texture.bump_map = NULL;
	plane->texture.texture_obj = NULL;
	plane->texture.checkered = 0;
	// Init point
	initialize_coordinate(&plane->point, params[1]);
	// Init normal
	initialize_coordinate(&plane->normal, params[2]);
	plane->normal = unit_vector(plane->normal);
	// Color
	initialize_coordinate(&plane->albedo, params[3]);
	plane->albedo = vec3_div_d(plane->albedo, 255.0);
	// Texture
	initialize_texture(&plane->texture, params, 4);
	shift = get_index_shift(&plane->texture);
	// Phong
	initialize_phong(&plane->phong, params, 5, shift);
	// Bump map
	initialize_bump_map(&plane->texture, params, 9, shift);
	(m->objects_count)[i] -= 1;
}

void	initialize_cylinder(t_master *m, char **params)
{
	int			i;
	int			j;
	t_cylinder	*cylinder;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	cylinder = &((m->cylinders)[j]);
	cylinder->texture.bump_map = NULL;
	cylinder->texture.texture_obj = NULL;
	cylinder->texture.checkered = 0;
	// Init center
	initialize_coordinate(&cylinder->center, params[1]);
	initialize_coordinate(&cylinder->axisnormal, params[2]);
	cylinder->axisnormal = unit_vector(cylinder->axisnormal);
	cylinder->radius = ft_atod(params[3]) / 2;
	cylinder->height = ft_atod(params[4]);
	initialize_coordinate(&cylinder->albedo, params[5]);
	cylinder->albedo = vec3_div_d(cylinder->albedo, 255.0);
	// Texture
	initialize_texture(&cylinder->texture, params, 6);
	shift = get_index_shift(&cylinder->texture);
	// Phong
	initialize_phong(&cylinder->phong, params, 7, shift);
	// Bump map
	initialize_bump_map(&cylinder->texture, params, 11, shift);
	(m->objects_count)[i] -= 1;
}

void	initialize_cone(t_master *m, char **params)
{
	int			i;
	int			j;
	t_cone	*cone;
	int			shift;

	i = index_of((char **)(m->ids), params[0]);
	j = (m->objects_count)[i] - 1;
	cone = &((m->cones)[j]);
	cone->texture.bump_map = NULL;
	cone->texture.texture_obj = NULL;
	cone->texture.checkered = 0;
	// Init center
	initialize_coordinate(&cone->tip, params[1]);
	initialize_coordinate(&cone->axis, params[2]);
	cone->axis = unit_vector(cone->axis);
	cone->height = ft_atod(params[3]);
	cone->angle = ft_atod(params[4]);
	initialize_coordinate(&cone->albedo, params[5]);
	cone->albedo = vec3_div_d(cone->albedo, 255.0);
	// Texture
	initialize_texture(&cone->texture, params, 6);
	shift = get_index_shift(&cone->texture);
	// Phong
	initialize_phong(&cone->phong, params, 7, shift);
	// Bump map
	initialize_bump_map(&cone->texture, params, 11, shift);
	(m->objects_count)[i] -= 1;
}
