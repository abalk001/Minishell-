#include "../include/minishell.h"

void	error_ft(char *filenames)
{
	perror(filenames);
	status_fct(EXIT_FAILURE);
}

void	print_error_2(char *error, char *message)
{
	char	*res;
	size_t	i;
	size_t	j;

	res = ft_malloc(sizeof(char) * ft_strlen(error) + ft_strlen(message) + 1,
			0);
	j = 0;
	i = 0;
	while (j < ft_strlen(error))
	{
		res[i] = error[j];
		i++;
		j++;
	}
	j = 0;
	while (j < ft_strlen(message))
	{
		res[i] = message[j];
		i++;
		j++;
	}
	res[i] = 0;
	ft_putstr_fd(res, 2);
}

void	print_error_3(char *error, char *message, char *extras)
{
	char	*res;
	size_t	i;
	size_t	j;

	res = ft_malloc(sizeof(char) * ft_strlen(error) + ft_strlen(message)
			+ ft_strlen(extras) + 1, 0);
	j = 0;
	i = 0;
	while (j < ft_strlen(error))
		res[i++] = error[j++];
	j = 0;
	while (j < ft_strlen(message))
		res[i++] = message[j++];
	j = 0;
	while (j < ft_strlen(extras))
		res[i++] = extras[j++];
	res[i] = 0;
	ft_putstr_fd(res, 2);
}
