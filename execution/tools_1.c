#include "../include/minishell.h"

void	parent_wait(t_cmd *cmds, pid_t *pids)
{
	int	i;
	int	status;

	i = -1;
	while (++i < count_commands(cmds))
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			status_fct(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			status_fct(128 + WTERMSIG(status));
		}
	}
	if (WIFSIGNALED(status))
		write(1, "\n", 1);
}

void	close_pipes(int nbr, int (*pipes)[2], pid_t *pid, int flag)
{
	int	i;

	if (!pipes)
		return ;
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

void	exit_bis(int status)
{
	ft_malloc(0, 1);
	exit(status);
}
