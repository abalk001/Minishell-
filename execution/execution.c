#include "../include/minishell.h"

static int	init_pipe(t_cmd *cmds, int (*pipes)[2], pid_t *pids)
{
	int	i;
	int	k;

	i = -1;
	k = -1;
	while (++i < count_commands(cmds) - 1)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			while (++k < i)
			{
				close(pipes[k][0]);
				close(pipes[k][1]);
			}
			free(pipes);
			free(pids);
			return (1);
		}
	}
	return (0);
}

static int	handle_pids_pipes(t_cmd *cmds, int (*pipes)[2], pid_t *pids)
{
	if ((count_commands(cmds) - 1) > 0 && !pipes)
	{
		perror("malloc");
		return (1);
	}
	if (!pids)
	{
		free(pipes);
		perror("malloc");
		return (1);
	}
	return (0);
}

static void	one_cmd(t_cmd *cmds, char ***env)
{
	if (ft_strcmp(cmds->args[0], "cd") == 0)
		status_fct(cmd_cd(cmds->args, *env));
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		status_fct(export(cmds->args, env));
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		status_fct(env_local(*env));
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		status_fct(echo_local(cmds->args));
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		status_fct(pwd_local(*env));
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		status_fct(unset(cmds->args, env));
	if (ft_strcmp(cmds->args[0], "exit") == 0)
		status_fct(builtin_exit(cmds->args));
	return ;
}

void	execute_close(t_cmd *cmds, char ***env, int save)
{
	one_cmd(cmds, env);
	dup2(save, STDOUT_FILENO);
	close(save);
}

void	execute_pipe(t_cmd *cmds, char ***env)
{
	pid_t	*pids;
	int		n;
	int		save;
	int		(*pipes)[2];

	n = count_commands(cmds);
	if (n == 1 && cmds->is_builtin)
	{
		if (cmds->redirections)
		{
			save = dup(STDOUT_FILENO);
			if (redirec_here_and_there_one_cmd(cmds) == 1)
				return (dup2(save, STDOUT_FILENO), (void)close(save));
			execute_close(cmds, env, save);
		}
		else
			one_cmd(cmds, env);
		return ;
	}
	pipes = ft_malloc(sizeof(int [2]) * (n - 1), 0);
	pids = ft_malloc(sizeof(pid_t) * n, 0);
	if (handle_pids_pipes(cmds, pipes, pids) || init_pipe(cmds, pipes, pids))
		return ;
	return (freud(cmds, pids, env, pipes), close_pipes(n, pipes, pids, 1),
		parent_wait(cmds, pids), signals_restore_bis());
}
