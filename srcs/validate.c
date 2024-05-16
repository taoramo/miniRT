/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:21 by vshchuki          #+#    #+#             */
/*   Updated: 2024/05/15 16:44:15 by vshchuki         ###   ########.fr       */
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

	line = get_next_line(fd);
	while (line)
	{
		if (prepare_line(&line, fd) == EMPTY_LINE)
			continue ;
		if (validate_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
		{
			ft_printf("Failed to validate this line: %s\n", line);
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	i = -1;
	while (++i < N_OBJECT_TYPES && !objects_count[i])
		;
	if (i == N_OBJECT_TYPES)
		return (print_error("No objects"));
	ft_printf("Scene validated.\n");
	return (EXIT_SUCCESS);
}

int	allocate_object(int objects_count[], void **objects,
	t_object_type type, int size)
{
	if (objects_count[type])
	{
		*objects = malloc(size * objects_count[type]);
		if (!(*objects))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	allocate_objects(int objects_count[], t_master *m)
{
	if (allocate_object(objects_count, (void **)&m->lights, l,
			sizeof(t_light)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_object(objects_count, (void **)&m->spheres, sp,
			sizeof(t_sphere)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_object(objects_count, (void **)&m->planes, pl,
			sizeof(t_plane)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_object(objects_count, (void **)&m->cylinders, cy,
			sizeof(t_cylinder)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (allocate_object(objects_count, (void **)&m->cones, co,
			sizeof(t_cone)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	validate(const char *argv[], int objects_count[], const char *ids[])
{
	int			fd;

	if (validate_filename(argv[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error", 5);
		return (1);
	}
	if (validate_scene(objects_count, ids, fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(fd);
	return (EXIT_SUCCESS);
}
