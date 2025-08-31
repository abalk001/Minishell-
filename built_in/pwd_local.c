#include "../minishell.h"

int	pwd_local(char **args, char **envp)
{
	char	*pwd;

	(void)args;
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
		return (0);
	}
	printf("%s\n", pwd);
	return (0);
}
