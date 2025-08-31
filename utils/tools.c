#include "../include/minishell.h"

int	is_space(char c)
{
	if (c == ' ')
	{
		return (1);
	}
	return (0);
}

int	is_special(char *str)
{
	if (ft_strncmp(str, "<", 1) == 0 && ft_strlen(str) == 1)
	{
		return (1);
	}
	else if (ft_strncmp(str, ">", 1) == 0 && ft_strlen(str) == 1)
	{
		return (1);
	}
	else if (ft_strncmp(str, "<<", 2) == 0 && ft_strlen(str) == 2)
	{
		return (1);
	}
	else if (ft_strncmp(str, ">>", 2) == 0 && ft_strlen(str) == 2)
	{
		return (1);
	}
	return (0);
}

int	is_pipe(char *str)
{
	if (ft_strncmp(str, "|", 1) == 0 && ft_strlen(str) == 1)
	{
		return (1);
	}
	return (0);
}

int	is_ex_special(char *str)
{
	if ((ft_strncmp(str, ">>", 2) == 0 || ft_strncmp(str, "<<", 2) == 0)
		&& ft_strlen(str) == 2)
	{
		return (1);
	}
	return (0);
}

int	ft_strcmp(char *string1, char *string2)
{
	size_t	i;
	int		diff;

	i = 0;
	diff = 0;
	while (string1[i] && string2[i])
	{
		diff = (unsigned char)string1[i] - (unsigned char)string2[i];
		if (diff != 0)
			return (diff);
		i++;
	}
	return (diff);
}
