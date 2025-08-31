#include "../include/minishell.h"

static int	get_redirections_nbr(t_token *tokens)
{
	int	len;

	len = 0;
	while (tokens)
	{
		if (tokens->type == PIPE)
			break ;
		if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT
			|| tokens->type == APPEND || tokens->type == HEREDOC)
			len++;
		tokens = tokens->next;
	}
	return (len);
}

static void	update_redirection(t_token **last_token,
		t_redirection **redirection, t_token_type type, t_token_type file_type)
{
	*last_token = (*last_token)->next;
	(*redirection)->expanded = 1;
	if ((*last_token)->quoted != NONE)
	{
		(*redirection)->expanded = 0;
	}
	(*redirection)->type = type;
	(*redirection)->filenames = get_files(last_token, file_type);
}

static t_redirection	*get_redirection(t_token **last_token)
{
	t_redirection	*redirection;

	redirection = ft_malloc(sizeof(t_redirection), 0);
	redirection->filenames = NULL;
	redirection->expanded = 1;
	redirection->next = NULL;
	while (*last_token && (*last_token)->type != REDIRECT_OUT
		&& (*last_token)->type != REDIRECT_IN && (*last_token)->type != APPEND
		&& (*last_token)->type != HEREDOC)
		*last_token = (*last_token)->next;
	if (!*last_token)
		return (redirection);
	if ((*last_token)->type == REDIRECT_IN)
		update_redirection(last_token, &redirection, REDIRECT_IN, INFILE);
	else if ((*last_token)->type == REDIRECT_OUT)
		update_redirection(last_token, &redirection, REDIRECT_OUT, OUTFILE);
	else if ((*last_token)->type == APPEND)
		update_redirection(last_token, &redirection, APPEND, APPEND_FILE);
	else if ((*last_token)->type == HEREDOC)
		update_redirection(last_token, &redirection, HEREDOC, HEREDOC_DEL);
	return (redirection);
}

static void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
	free(tokens);
}

t_redirection	*get_redirections(t_token *tokens)
{
	t_redirection_data	redir_data;

	redir_data.head = NULL;
	redir_data.current = NULL;
	redir_data.redirections_len = get_redirections_nbr(tokens);
	redir_data.i = 0;
	redir_data.last_token = tokens;
	while (redir_data.i < redir_data.redirections_len)
	{
		redir_data.new_node = get_redirection(&redir_data.last_token);
		if (!redir_data.new_node)
		{
			free_tokens(tokens);
			status_fct(STATUS_OTHER);
			return (NULL);
		}
		if (!redir_data.head)
			redir_data.head = redir_data.new_node;
		else
			redir_data.current->next = redir_data.new_node;
		redir_data.current = redir_data.new_node;
		redir_data.i++;
	}
	return (redir_data.head);
}
