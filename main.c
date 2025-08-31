#include "include/minishell.h"

static void	setup_prompt_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler_parent;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) < 0)
	{
		perror("sigaction");
		ft_malloc(0, 1);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		ft_malloc(0, 1);
		exit(EXIT_FAILURE);
	}
}

int	status_fct(int exit_code)
{
	static int	exit_co = 0;
	int			tmp;

	if (exit_code == -1)
		return (exit_co);
	else
	{
		exit_co = exit_code;
		tmp = exit_co;
	}
	return (tmp);
}

// void	print_cmd(t_cmd *cmds)
// {
// 	t_cmd	*tmp;
// 	int		i;

// 	tmp = cmds;
// 	while (tmp)
// 	{
// 		printf("This command is builtin {%d} has :\n", tmp->is_builtin);
// 		i = 0;
// 		while (tmp->args[i])
// 		{
// 			printf("\tArgument : {%s}\n", tmp->args[i]);
// 			i++;
// 		}
// 		i = 0;
// 		printf("\tRedirectons :\n");
// 		while (tmp->redirections)
// 		{
// 			printf("\t\tFile : {%s} with type {%d} and expanded {%d}\n",
// 				tmp->redirections->filenames, tmp->redirections->type,
// 				tmp->redirections->expanded);
// 			tmp->redirections = tmp->redirections->next;
// 		}
// 		tmp = tmp->next;
// 	}
// }

static int	open_her(t_cmd *cmds, char **envp)
{
	t_cmd			*tmp;
	t_redirection	*redir;
	int				has_heredoc;

	tmp = cmds;
	has_heredoc = 0;
	while (tmp)
	{
		redir = tmp->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				has_heredoc = 1;
				break ;
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
	if (has_heredoc)
		if (heredoc(cmds, envp) < 0)
			return (1);
	return (0);
}

static int	main_helper_helper(t_cmd *cmds, char ***envp)
{
	if (open_her(cmds, *envp))
		return (3);
	if (cmds->args || cmds->redirections)
	{
		if ((count_commands(cmds) == 1 && !cmds->args[0])
			&& !cmds->redirections)
			return (3);
		execute_pipe(cmds, envp);
	}
	return (0);
}

int	main(int ac, char **av, char **o_envp)
{
	t_cmd	*cmds;
	char	*input;
	char	**envp;

	int (status), (check);
	(void)ac;
	(void)av;
	envp = clone_env(o_envp);
	setup_prompt_signals();
	status = 0;
	while (1)
	{
		check = main_helper(&input);
		if (check == 3)
			continue ;
		if (check == 4)
			break ;
		cmds = parse_pipe(input, envp);
		free(input);
		if (cmds != NULL)
			if (main_helper_helper(cmds, &envp) == 3)
				continue ;
	}
	return (ft_malloc(0, 1), rl_clear_history(), status_fct(-1));
}
