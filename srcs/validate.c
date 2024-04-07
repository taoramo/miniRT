/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:21 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/07 22:59:41 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Identifier
// A, C, L, sp, pl, cy

// A 0.2 255,255,255
// C -50.0,0,20 0,0,1 70
// L -40.0,50.0,0.0 0.6 10,0,255
// sp 0.0,0.0,20.6 12.6 10,0,255
// pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
// cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255

int	prepare_line(char **line, int fd)
{
	char		*tmp;

	tmp = ft_strtrim(*line, " \f\n\r\t\v");
	free(*line);
	*line = tmp;
	if (!tmp[0])
	{
		*line = get_next_line(fd);
		return (EMPTY_LINE);
	}
	replace_whitespaces(*line);
	return (EXIT_SUCCESS);
}

int	validate_scene(int objects_count[], int fd)
{
	char		*line;
	int			i;
	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "L", "sp", "pl", "cy"};

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (prepare_line(&line, fd) == EMPTY_LINE)
			continue ;
		printf("%d. %s\n", ++i, line);
		if (validate_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
		{
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	printf("Scene validated.\n");
	return (EXIT_SUCCESS);
}

/* allocate_objects(int objects_count[])
{
	if (objects_count[A])
		allocate_ambient_light();
	if (objects_count[C])
		allocate_camera();
	if (objects_count[L])
		allocate_light();
	if (objects_count[sp])
		allocate_sphere();
	if (objects_count[pl])
		allocate_plane();
	if (objects_count[cy])
		allocate_cylinder();
	// if (objects_count[co])
	// 	allocate_cone();
} */

int	main(int argc, char const *argv[])
{
	int	objects_count[N_OBJECT_TYPES];

	ft_bzero(objects_count, sizeof(int) * N_OBJECT_TYPES);
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	if (validate_scene(objects_count, fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);

	// Test object count
	// int i = 0;
	// while (i < N_OBJECT_TYPES)
	// {
	// 	printf("%d : %d\n", i, objects_count[i]);
	// 	i++;
	// }

	return (EXIT_SUCCESS);
}
