#include "../include/minishell.h"

static char	*get_env_value(char *name, char **envp)
{
	int	len;
	int	i;

	len = ft_strlen(name);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
	}
	return (0);
}

int	pwd_local(char **envp)
{
	char	*pwd;

	pwd = get_env_value("PWD", envp);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			write(2, "failed pwd\n", 12);
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
		return (0);
	}
	printf("%s\n", pwd);
	return (0);
}
