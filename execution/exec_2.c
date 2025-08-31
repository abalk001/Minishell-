#include "../include/minishell.h"

static void	build_in_pipe(t_cmd *cur, char ***env)
{
	if (ft_strcmp(cur->args[0], "cd") == 0)
		exit_bis(cmd_cd(cur->args, *env));
	else if (ft_strcmp(cur->args[0], "exit") == 0)
		exit_bis(builtin_exit(cur->args));
	else if (ft_strcmp(cur->args[0], "export") == 0)
		exit_bis(export(cur->args, env));
	else if (ft_strcmp(cur->args[0], "env") == 0)
		exit_bis(env_local(*env));
	else if (ft_strcmp(cur->args[0], "echo") == 0)
		exit_bis(echo_local(cur->args));
	else if (ft_strcmp(cur->args[0], "pwd") == 0)
		exit_bis(pwd_local(*env));
	else if (ft_strcmp(cur->args[0], "unset") == 0)
		exit_bis(unset(cur->args, env));
}

static void	close_pipes_child(int nbr, int (*pipes)[2])
{
	int	i;

	i = -1;
	while (++i < nbr - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

static void	child_stuff(int (*pipes)[2], t_cmd *cur, char ***env, int i, int n)
{
	setup_child_signals();
	if (i > 0)
	{
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			perror("dup2");
	}
	if (i < n - 1)
	{
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			perror("dup2");
	}
	close_pipes_child(n, pipes);
	redirec_here_and_there(cur);
	if (!cur->args || !cur->args[0] || !cur)
	{
		ft_malloc(0, 1);
		exit(EXIT_SUCCESS);
	}
	if (cur->is_builtin)
		build_in_pipe(cur, env);
	else if (cur->args != NULL)
		alpha(cur, env);
}

static void	signals_bis(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	freud(t_cmd *cmds, pid_t *pids, char ***env, int (*pipes)[2])
{
	t_cmd	*cur;
	int		i;
	pid_t	pid;

	cur = cmds;
	i = -1;
	while (++i < count_commands(cmds))
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			close_pipes(count_commands(cmds), pipes, pids, 0);
			return ;
		}
		else if (pid == 0)
			child_stuff(pipes, cur, env, i, count_commands(cmds));
		else
		{
			signals_bis();
			pids[i] = pid;
		}
		if (cur)
			cur = cur->next;
	}
}
