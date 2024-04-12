/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:21 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/12 16:54:05 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	prepare_line(char **line, int fd)
{
	char		*tmp;

	tmp = ft_strtrim(*line, " \f\n\r\t\v");
	free(*line);
	*line = tmp;
	if (!tmp || !tmp[0])
	{
		free(*line);
		*line = get_next_line(fd);
		return (EMPTY_LINE);
	}
	replace_whitespaces(*line);
	return (EXIT_SUCCESS);
}

int	validate_scene(int objects_count[], const char *ids[], int fd)
{
	char		*line;
	int			i;

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (prepare_line(&line, fd) == EMPTY_LINE)
			continue ;
		printf("%d. %s\n", ++i, line); // // leads to Conditional jump or move depends on uninitialised value(s) and Uninitialised value was created by a stack allocation
		if (validate_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
		{
			printf("Failed to validate this line: %s\n", line);
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	printf("Scene validated.\n");
	return (EXIT_SUCCESS);
}

int	allocate_objects(int objects_count[], t_master *m)
{
	if (objects_count[L])
	{
		m->lights = malloc(sizeof(t_light) * objects_count[L]);
		if (!m->lights)
			return (EXIT_FAILURE);
	}
	if (objects_count[sp])
	{
		m->spheres = malloc(sizeof(t_sphere) * objects_count[sp]);
		if (!m->spheres)
			return (EXIT_FAILURE);
	}
	if (objects_count[pl])
	{
		m->planes = malloc(sizeof(t_plane) * objects_count[pl]);
		if (!m->planes)
			return (EXIT_FAILURE);
	}
	if (objects_count[cy])
	{
		m->cylinders = malloc(sizeof(t_cylinder) * objects_count[cy]);
		if (!m->cylinders)
			return (EXIT_FAILURE);
	}
	if (objects_count[co])
	{
		m->cones = malloc(sizeof(t_cone) * objects_count[co]);
		if (!m->cones)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	validate(const char *argv[], int objects_count[], const char *ids[])
{
	int			fd;

	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return (1);
	}
	if (validate_scene(objects_count, ids, fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}

// int	main(int argc, char const *argv[])
// {
// 	int			fd;
// 	int			objects_count[N_OBJECT_TYPES];
// 	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "L", "sp", "pl", "cy"};
// 	t_master	m;
// 	// t_camera	camera;
// 	// mlx_t		*mlx;

// 	ft_bzero(objects_count, sizeof(int) * N_OBJECT_TYPES);
// 	if (argc < 2)
// 	{
// 		printf("Usage: %s <filename>\n", argv[0]);
// 		return (1);
// 	}
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error");
// 		return (1);
// 	}
// 	if (validate_scene(objects_count, ids, fd) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	close(fd);

// 	if (allocate_objects(objects_count, &m) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);

// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("Error");
// 		return (1);
// 	}
// 	// initialize_scene(&m, &camera, mlx);
// 	close(fd);

// /* 	Test object count
// 	int i = 0;
// 	while (i < N_OBJECT_TYPES)
// 	{
// 		printf("%d : %d\n", i, objects_count[i]);
// 		i++;
// 	} */




// 	return (EXIT_SUCCESS);
// }
