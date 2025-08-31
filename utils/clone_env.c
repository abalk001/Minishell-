#include "../include/minishell.h"

char	**clone_env(char **envp)
{
	int		i;
	int		len;
	char	**clone;

	i = 0;
	len = 0;
	while (envp[len])
		len++;
	clone = ft_malloc((len + 1) * sizeof(char *), 0);
	while (i < len)
	{
		clone[i] = ft_strdup(envp[i]);
		i++;
	}
	clone[i] = NULL;
	return (clone);
}

char	*extract_var_name(char *start)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (start[i] && (ft_isalnum(start[i]) || start[i] == '_'))
		i++;
	var = malloc(sizeof(char) * (i + 1));
	if (!var)
		return (NULL);
	j = 0;
	while (j < i)
	{
		var[j] = start[j];
		j++;
	}
	var[i] = '\0';
	return (var);
}

char	**clone_array(char **arr)
{
	int		n;
	int		i;
	char	**clone;

	i = 0;
	n = 0;
	while (arr[n])
		n++;
	clone = ft_malloc(sizeof(char *) * (n + 1), 0);
	while (i < n)
	{
		clone[i] = ft_strdup(arr[i]);
		i++;
	}
	clone[n] = NULL;
	return (clone);
}
