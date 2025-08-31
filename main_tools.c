#include "include/minishell.h"

static int	is_misquoted(char *line)
{
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"'))
		{
			if (quote == 0)
				quote = line[i];
			else if (quote == line[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}

static void	handle_eof(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
}

int	main_helper(char **input)
{
	*input = readline("Unishell> ");
	if (!*input)
	{
		handle_eof();
		return (4);
	}
	if (ft_strncmp(*input, "", 1) == 0)
	{
		free(*input);
		return (3);
	}
	if (**input)
		add_history(*input);
	if (is_misquoted(*input))
	{
		ft_putstr_fd(ERROR_LINE, 2);
		status_fct(STATUS_OTHER);
		free(*input);
		return (3);
	}
	return (0);
}
