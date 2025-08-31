#include "../include/minishell.h"

static int	get_files_nb(t_token *tokens, t_token_type type)
{
	int	len;

	len = 0;
	while (tokens && tokens->type == type)
	{
		len++;
		tokens = tokens->next;
	}
	return (len);
}

char	*get_files(t_token **tokens_ptr, t_token_type type)
{
	char	*filenames;
	int		files_nbr;

	files_nbr = get_files_nb(*tokens_ptr, type);
	filenames = ft_strdup((*tokens_ptr)->value);
	(*tokens_ptr) = (*tokens_ptr)->next;
	return (filenames);
}
