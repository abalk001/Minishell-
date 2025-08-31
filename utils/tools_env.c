#include "../minishell.h"

char	*get_env_value(char *name, char **envp)
{
	int len;
	int i;

	len = ft_strlen(name);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return envp[i] + len + 1;
	}
	return (0);
}
