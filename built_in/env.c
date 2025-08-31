#include "../minishell.h"

int	env_local(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		printf("%s\n", envp[i]);
	return (0);
}

char	*find_env_value(const char *key, char **envp)
{
	int	key_len;
	int	i;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(key, envp[i], key_len) == 0 && envp[i][key_len] == '=')
			return (envp[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
