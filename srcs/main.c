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
		printf("Usage: %s <filename>\n", argv[0]);
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
	free_all(&m);
	return (EXIT_SUCCESS);
}
