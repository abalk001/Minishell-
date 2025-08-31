

#include "../minishell.h"

static void	update_type(t_token *token, t_token_type type)
{
	if (token->type == WORD)
		token->type = type;
}

void	update_tokens(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == REDIRECT_IN)
			update_type(tmp->next, INFILE);
		else if (tmp->type == REDIRECT_OUT)
			update_type(tmp->next, OUTFILE);
		else if (tmp->type == APPEND)
			update_type(tmp->next, APPEND_FILE);
		else if (tmp->type == HEREDOC)
			update_type(tmp->next, HEREDOC_DEL);
		tmp = tmp->next;
	}
}

static t_token	*add_token(char *value, t_token *next)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->value = parsing_strdup(value);
	token->type = WORD;
	token->next = next;
	token->quoted = NONE;
	token->is_joignable = 0;
	return (token);
}

static t_token	*modify_token(t_token *tmp)
{
	t_modify_data	modify_data;

	modify_data.new_head = NULL;
	modify_data.curr = NULL;
	modify_data.splitted = ft_split(tmp->value, ' ');
	if (!modify_data.splitted)
		return (NULL);
	modify_data.len = 0;
	while (modify_data.splitted[modify_data.len])
		modify_data.len++;
	modify_data.i = 0;
	while (modify_data.i < modify_data.len)
	{
		modify_data.token = add_token(modify_data.splitted[modify_data.i],
				NULL);
		if (!modify_data.token)
		{
			free_all(modify_data.splitted);
			return (NULL);
		}
		if (!modify_data.new_head)
			modify_data.new_head = modify_data.token;
		else
			modify_data.curr->next = modify_data.token;
		modify_data.curr = modify_data.token;
		modify_data.i++;
	}
	modify_data.curr->next = tmp->next;
	free_all(modify_data.splitted);
	free(tmp->value);
	free(tmp);
	return (modify_data.new_head);
}

void	join_tokens(t_token **tokens)
{
	t_token	*tmp;
	t_token	*last;
	t_token	*new_tokens;

	tmp = *tokens;
	last = NULL;
	while (tmp)
	{
		if (tmp->quoted == NONE && !is_one_word(tmp->value))
		{
			new_tokens = modify_token(tmp);
			if (!new_tokens)
				return ;
			if (last == NULL)
				*tokens = new_tokens;
			else
				last->next = new_tokens;
			tmp = new_tokens;
		}
		last = tmp;
		tmp = tmp->next;
	}
}
