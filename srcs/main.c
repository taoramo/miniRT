/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toramo <toramo.student@hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:40:22 by toramo            #+#    #+#             */
/*   Updated: 2024/05/15 16:40:23 by toramo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	handle_esc(mlx_key_data_t keydata, void *m)
{
	if (keydata.key == 256)
	{
		mlx_terminate(((t_master *)m)->mlx);
		free_all(m);
		exit(EXIT_SUCCESS);
	}
}

void	delete_cone_cylinder_textures(t_master *m)
{
	unsigned int	i;

	i = 0;
	while (i < m->n_cones)
	{
		mlx_delete_texture(m->cones[i].texture.texture_obj);
		mlx_delete_texture(m->cones[i].texture.bump_map);
		i++;
	}
	i = 0;
	while (i < m->n_cylinders)
	{
		mlx_delete_texture(m->cylinders[i].texture.texture_obj);
		mlx_delete_texture(m->cylinders[i].texture.bump_map);
		i++;
	}
}

void	delete_textures(t_master *m)
{
	unsigned int	i;

	i = 0;
	while (i < m->n_spheres)
	{
		mlx_delete_texture(m->spheres[i].texture.texture_obj);
		mlx_delete_texture(m->spheres[i].texture.bump_map);
		i++;
	}
	i = 0;
	while (i < m->n_planes)
	{
		mlx_delete_texture(m->planes[i].texture.texture_obj);
		mlx_delete_texture(m->planes[i].texture.bump_map);
		i++;
	}
	delete_cone_cylinder_textures(m);
}

int	main(int argc, char const *argv[])
{
	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "l", "sp", "pl", "cy", "co"};
	t_master	m;
	mlx_t		*mlx;

	mlx = NULL;
	initialize_master_struct(&m, ids);
	ft_bzero(m.objects_count, sizeof(int) * N_OBJECT_TYPES);
	if (argc < 2)
	{
		ft_printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	if (validate(argv, m.objects_count, ids) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_objects(m.objects_count, &m) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	initialize(&m, &mlx, argv);
	mlx_key_hook(mlx, handle_esc, &m);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	if (render(&m, mlx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	delete_textures(&m);
	free_all(&m);
	return (EXIT_SUCCESS);
}
