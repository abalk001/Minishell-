#include "../include/minishell.h"

int	is_valid(const char *arg)
{
	int	i;

	i = 1;
	if ((!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
