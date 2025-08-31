#include "../minishell.h"

int	get_tokens_size(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	update_token(t_token *token, char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		total;

	total = ft_strlen(str) + ft_strlen(token->value);
	new_str = malloc(sizeof(char) * (total + 1));
	i = 0;
	while (i < total)
	{
		j = 0;
		while (token->value[j])
			new_str[i++] = token->value[j++];
		j = 0;
		while (str[j])
			new_str[i++] = str[j++];
	}
	new_str[i] = 0;
	free(token->value);
	token->value = parsing_strdup(new_str);
	free(new_str);
}

void	join_all(t_token **tokens_ptr)
{
	t_token	*tokens;
	t_token	*tmp;
	char	*to_join_str;

	tokens = *tokens_ptr;
	while (tokens)
	{
		if (tokens->is_joignable)
		{
			tmp = tokens->next;
			to_join_str = tmp->value;
			update_token(tokens, to_join_str);
			tokens->is_joignable = tmp->is_joignable;
			tokens->quoted = tmp->quoted;
			tokens->next = tmp->next;
			free(tmp->value);
			free(tmp);
		}
		else
			tokens = tokens->next;
	}
}

void	clear_tokens(t_token **tokens)
{
	t_token	*tmp;
	t_token	*last;
	t_token	*to_delete;

	tmp = *tokens;
	last = NULL;
	while (tmp)
	{
		if (ft_strlen(tmp->value) == 0)
		{
			if (tmp->quoted == NONE)
			{
				to_delete = tmp;
				tmp = tmp->next;
				if (last == NULL)
					*tokens = tmp;
				else
					last->next = tmp;
				free(to_delete->value);
				free(to_delete);
			}
			else
			{
				tmp = tmp->next;
			}
			
		}
		else
		{
			last = tmp;
			tmp = tmp->next;
		}
	}
}

t_cmd	*parse_pipe(char *line, char **envp)
{
	t_cmd	*cmds;
	t_token	*tokens;

	cmds = NULL;
	tokens = tokenize(line);
	if (tokens == NULL)
		return (NULL);
	if (validate_pipe(tokens) == -1)
    {
        free_tokens(tokens);
		return (NULL);
    }
	update_tokens(&tokens);
	expand_all(&tokens, envp);
	join_tokens(&tokens);
	join_all(&tokens);
	printf("This token is {%s} quoted {%d}\n", tokens->value, tokens->quoted);
	clear_tokens(&tokens);
	if (tokens == NULL)
		return (NULL);
	cmds = build_cmds(tokens);
	if (cmds == NULL && status_fct(-1) == 2)
	{
		ft_malloc(0, 1);
		return (NULL);
	}
	free_tokens(tokens);
	return (cmds);
}
