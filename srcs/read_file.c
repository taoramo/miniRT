#include "miniRT.h"


bool is_capital(char c)
{
	return (c >= 'A' && c <= 'Z');
}
/**
 * Checks for \t (ht) - tab, \n (nl) - new line, \v (vt) vertical tab, 
 * \f (np) - feed form aka new page, \r (cr) - carriage return
*/
bool is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

void	replace_whitespaces(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			line[i] = ' ';
		i++;
	}
}

// Identifier
// A, C, L, sp, pl, cy

// A 0.2 255,255,255
// C -50.0,0,20 0,0,1 70
// L -40.0,50.0,0.0 0.6 10,0,255
// sp 0.0,0.0,20.6 12.6 10,0,255
// pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
// cy 50.0,0.0,20.6 0.0,0.0,1.0 14.2 21.42 10,0,255

int	verify_ambient_light(char **params)
{
	if (ft_strlen((char *)params) != 2)
	{
		printf("Error: Ambient light must have 2 parameters.\n");
		return (EXIT_FAILURE);
	}
	// verify float string
	if (ft_atod(params[0]) < 0 || ft_atod(params[0]) > 1)
	{
		printf("Error: Ambient light intensity must be between 0 and 1.\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/* int	verify_line_parameters(char *line, t_object_type id)
{
	char	**params;

	replace_whitespaces(line);
	params = ft_split(line, ' ');
	if (id == A)
		return (verify_ambient_light(params));
	else if (id == C)
		return (verify_camera(params));
	else if (id == L)
		return (verify_light(params));
	else if (id == sp)
		return (verify_sphere(params));
	else if (id == pl)
		return (verify_plane(params));
	else if (id == cy)
		return (verify_cylinder(params));
	return (EXIT_FAILURE);
} */

int	check_unique(int objects_count[], t_object_type id)
{
	if (objects_count[id] > 1)
	{
		printf("Error: Elements which are defined by a capital "
			"letter can only be declared once in the scene.\n");
		return (1);
	}
	return (0);
}

int	check_line_identifier(char *line, int objects_count[], const char *ids[])
{
	char *id_with_space;
	t_object_type id;

	id = 0;
	while (id < N_OBJECT_TYPES)
	{
		id_with_space = ft_strjoin(ids[id], " ");
		if (!ft_strncmp(line, id_with_space, ft_strlen(ids[id]) + 1))
		{
			objects_count[id]++;
			free(id_with_space);
			if (is_capital(ids[id][0]) &&
				check_unique(objects_count, id) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			// verify if the line parameters are valid
			return (EXIT_SUCCESS);
		}
		id++;
	}
	if (id == N_OBJECT_TYPES)
	{
		printf("Error: Invalid identifier.\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// void	parse(t_master m, int objects_count[], int fd)
int	parse(int objects_count[], int fd)
{
	char	*line;
	char	*tmp;
	int		i;
	const char	*ids[N_OBJECT_TYPES] = {"A", "C", "L", "sp", "pl", "cy"};

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		tmp = ft_strtrim(line, " \f\n\r\t\v");
		free(line);
		line = tmp;
		if (!tmp[0])
		{
			line = get_next_line(fd);
			continue ;
		}
		printf("%d. ", ++i);
		printf("%s\n", line);

		if (check_line_identifier(line, objects_count, ids) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		free(line);
		line = get_next_line(fd);
	}
	return (EXIT_SUCCESS);
}

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
	if (parse(objects_count, fd) == 1)
		return (1);

	// Test object count
	// int i = 0;
	// while (i < N_OBJECT_TYPES)
	// {
	// 	printf("%d : %d\n", i, objects_count[i]);
	// 	i++;
	// }

	return (0);
}
