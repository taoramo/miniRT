#include "miniRT.h"

int print_error(char *err)
{
	printf("Error:\n");
	if (err[0])
		printf("%s\n", err);
	return (EXIT_FAILURE);
}

void	replace_whitespaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			line[i] = ' ';
		i++;
	}
}
