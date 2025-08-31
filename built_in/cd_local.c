#include "../include/minishell.h"

static void	addstr(char *dest, const char *src)
{
	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

static char	*ft_pwd(char *pwd, char *cwd)
{
	char	*new_pwd;

	new_pwd = ft_malloc(ft_strlen(pwd) + ft_strlen(cwd) + 1, 0);
	ft_strlcpy(new_pwd, pwd, ft_strlen(pwd) + 1);
	addstr(new_pwd, cwd);
	return (new_pwd);
}

static void	update_path(char **envp, char *pwd)
{
	char	*cwd;
	char	*new_pwd;
	int		i;

	i = 0;
	cwd = getcwd(NULL, 0);
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], pwd, ft_strlen(pwd)) == 0)
		{
			if (!cwd)
			{
				new_pwd = ft_pwd(envp[i], "/..");
				envp[i] = ft_strdup(new_pwd);
				free(cwd);
				return ;
			}
			new_pwd = ft_pwd(pwd, cwd);
			envp[i] = ft_strdup(new_pwd);
			free(cwd);
			return ;
		}
	}
}

int	cmd_cd(char **args, char **envp)
{
	char	*var;

	if (!args[1])
	{
		var = getenv("USER");
		if (!var)
			return (ft_putstr_fd("environnement not found", 2), 1);
		var = ft_strjoin3("/home/", var, "/");
		update_path(envp, "OLDPWD=");
		chdir(var);
		update_path(envp, "PWD=");
		return (1);
	}
	if (args[2])
	{
		write(2, "too many arguments\n", 20);
		return (1);
	}
	update_path(envp, "OLDPWD=");
	if (chdir(args[1]) != 0)
		return (perror("minishell: cd"), 1);
	update_path(envp, "PWD=");
	return (0);
}
