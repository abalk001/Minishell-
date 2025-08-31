#include "../include/minishell.h"

int	count_commands(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		num_cmds;

	num_cmds = 0;
	tmp = cmds;
	while (tmp)
	{
		num_cmds++;
		tmp = tmp->next;
	}
	return (num_cmds);
}

int	is_one_word(char *str)
{
	int		i;
	char	**splitted;

	splitted = ft_split(str, ' ');
	i = 0;
	while (splitted[i])
		i++;
	if (i > 1)
		return (0);
	return (1);
}

void	sigint_handler_parent(int signo)
{
	(void)signo;
	status_fct(STATUS_SIGINT);
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	setup_child_signals(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
	{
		perror("signal SIGINT child");
		return (-1);
	}
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	{
		perror("signal SIGQUIT child");
		return (-1);
	}
	return (0);
}

void	signals_restore_bis(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler_parent;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
