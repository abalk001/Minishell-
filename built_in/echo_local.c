#include "../minishell.h"

int	echo_local(char **args)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	if (!args[1])
		return (write(STDOUT_FILENO, "\n", 1), 0);
	while (args[1][0] == '-' && args[1][++i])
	{
		if (args[1][i] == 'n')
			newline = 0;
	}
	i = 0;
	if (args[1] && (newline == 1) == 0)
		i++;
	while (args[++i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

