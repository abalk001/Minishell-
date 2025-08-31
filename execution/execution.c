#include "../minishell.h"

static int	init_pipe(t_cmd *cmds, int (*pipes)[2], pid_t *pids)
{
	int i;
	int	k;

	i = -1;
	k = -1;
	while (++i < count_commands(cmds) - 1)
	{
		if (pipe(pipes[i]) < 0) 
		{
			perror("pipe");
			while(++k < i)
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
static void	signals_restore()
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler_parent;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
static void	parent_wait(t_cmd *cmds, pid_t *pids)
{
	int	i;
	int status;

	i = -1;
	while(++i < count_commands(cmds))
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			status_fct(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			status_fct(128 + WTERMSIG(status));
		}
	}
	if(WIFSIGNALED(status))
		write(1, "\n", 1);
}
void execute_pipe(t_cmd *cmds, char ***env)
{
	int (*pipes)[2];
	pid_t *pids;
	
	if (count_commands(cmds) == 1 && cmds->is_builtin)
	{
		one_cmd(cmds, env);
		return;
	}
	pipes = malloc(sizeof(int[2]) * (count_commands(cmds) - 1));
	pids = malloc(sizeof(pid_t) * count_commands(cmds));
	if (handle_pids_pipes(cmds, pipes, pids))
		return;
	if (init_pipe(cmds, pipes, pids))
		return ;
	freud(cmds, pids, env, pipes);
	close_pipes(count_commands(cmds), pipes, pids, 1);
	parent_wait(cmds, pids);
	signals_restore();
	free(pids);
	free(pipes);
}
