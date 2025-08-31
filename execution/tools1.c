#include "../minishell.h"

void	close_pipes(int nbr, int (*pipes)[2], pid_t *pid, int flag)
{
    int i;
    
    if (!pipes)
        return;
        
    i = -1;
    while (++i < nbr - 1)
    {
        if (pipes[i][0] != -1)
        {
            close(pipes[i][0]);
            pipes[i][0] = -1;
        }
        if (pipes[i][1] != -1)
        {
            close(pipes[i][1]);
            pipes[i][1] = -1;
        }
    }
    
    if (flag == 0)
    {
        free(pipes);
        if (pid)
            free(pid);
    }
}
void	build_in_pipe(t_cmd *cur, char ***env)
{
	if (ft_strcmp(cur->args[0], "cd") == 0)
        exit(cmd_cd(cur->args, *env));
    else if (ft_strcmp(cur->args[0], "exit") == 0)
        exit(builtin_exit(cur->args,*env));
    else if (ft_strcmp(cur->args[0], "export") == 0)
        exit(export(cur->args, env));
    else if (ft_strcmp(cur->args[0], "env") == 0)
        exit(env_local(*env));
    else if (ft_strcmp(cur->args[0], "echo") == 0)
        exit(echo_local(cur->args));
    else if (ft_strcmp(cur->args[0], "pwd") == 0)
        exit(pwd_local(cur->args, *env));
    else if (ft_strcmp(cur->args[0], "unset") == 0)
        exit(unset(cur->args, env));
}

void	beta(t_cmd *cur, char ***env, char *path)
{
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd(cur->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(path);
		exit(127);
	}
	if (execve(path, cur->args, *env) == -1)
	{
		perror("execve");
		free(path);
		exit(EXIT_FAILURE);
	}
	free(path);
	exit(EXIT_SUCCESS);
}

void	alpha(t_cmd *cur, char ***env)
{
	struct stat	st;
	char *path;

	if (stat(cur->args[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(cur->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	path = find_path(cur->args[0], *env);
	if (!path)
	{
		ft_putstr_fd(cur->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
    }
	if (stat(path, &st) == -1)
	{
		ft_putstr_fd(cur->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(path);
		exit(127);
	}
	beta(cur, env, path);
}
void	one_cmd(t_cmd *cmds, char ***env)
{
	int		save;

	save = dup(STDOUT_FILENO);
	redirec_here_and_there(cmds);
	if (ft_strcmp(cmds->args[0], "cd") == 0)
		status_fct(cmd_cd(cmds->args, *env));
	else if (ft_strcmp(cmds->args[0], "exit") == 0)
		status_fct(builtin_exit(cmds->args, *env));
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		status_fct(export(cmds->args, env));
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		status_fct(env_local(*env));
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		status_fct(echo_local(cmds->args));
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		status_fct(pwd_local(cmds->args, *env));
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		status_fct(unset(cmds->args, env));
	dup2(save, STDOUT_FILENO);
    close(save);
	return;
}
