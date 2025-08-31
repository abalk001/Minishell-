#include "../include/minishell.h"

static int	get_args_len(t_token *tokens, t_token_type type)
{
	int	len;

	len = 0;
	while (tokens)
	{
		if (ft_strncmp(tokens->value, "|", 1) == 0 && tokens->type == PIPE)
		{
			break ;
		}
		if (tokens->type == type)
		{
			len++;
		}
		tokens = tokens->next;
	}
	return (len);
}

char	**get_args(t_token *tokens, t_token_type type)
{
	char	**args;
	int		i;
	int		args_nbr;

	args_nbr = get_args_len(tokens, type);
	args = ft_malloc(sizeof(char *) * (args_nbr + 1), 0);
	i = 0;
	while (i < args_nbr)
	{
		if (tokens->type == type)
		{
			args[i] = ft_strdup((tokens)->value);
			if (!args[i])
			{
				ft_malloc(0, 1);
				status_fct(STATUS_OTHER);
				return (NULL);
			}
			i++;
		}
		tokens = (tokens)->next;
	}
	args[i] = NULL;
	return (args);
}
