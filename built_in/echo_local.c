#include "../include/minishell.h"

void	write_newline(int newline)
{
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

int	echo_local(char **args)
{
	int	j;
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	write_newline(newline);
	return (0);
}
