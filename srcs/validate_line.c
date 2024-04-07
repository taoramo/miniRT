/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vshchuki <vshchuki@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:59:23 by vshchuki          #+#    #+#             */
/*   Updated: 2024/04/07 22:59:23 by vshchuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	validate_line_parameters(char *line, t_object_type id)
{
	char	**params;
	int		is_exit_failure;

	is_exit_failure = EXIT_FAILURE;
	params = ft_split(line, ' ');
	if (id == A)
		is_exit_failure = validate_ambient_light(params + 1);
	else if (id == C)
		is_exit_failure = validate_camera(params + 1);
	else if (id == L)
		is_exit_failure = validate_light(params + 1);
	else if (id == sp)
		is_exit_failure = validate_sphere(params + 1);
	else if (id == pl)
		is_exit_failure = validate_plane(params + 1);
	else if (id == cy)
		is_exit_failure = validate_cylinder(params + 1);
	// else if (id == co)
	// 	is_exit_failure = validate_cone(params + 1);
	free_split(params);
	return (is_exit_failure);
	// return (is_exit_failure && print_error("Invalid parameters."));
}

int	validate_unique(int objects_count[], t_object_type id)
{
	if (objects_count[id] > 1)
		return (print_error("Elements which are defined by a capital "
				"letter can only be declared once in the scene."));
	return (0);
}

int	validate_line_identifier(char *line, int objects_count[], const char *ids[])
{
	char			*id_with_space;
	t_object_type	id;

	id = 0;
	while (id < N_OBJECT_TYPES)
	{
		id_with_space = ft_strjoin(ids[id], " ");
		if (!ft_strncmp(line, id_with_space, ft_strlen(ids[id]) + 1))
		{
			objects_count[id]++;
			free(id_with_space);
			if (is_capital(ids[id][0]) &&
				validate_unique(objects_count, id) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			return (validate_line_parameters(line, id));
		}
		id++;
	}
	if (id == N_OBJECT_TYPES)
		return (print_error("Invalid identifier."));
	return (EXIT_SUCCESS);
}
