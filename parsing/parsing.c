#include "../include/minishell.h"

static void	update_token(t_token *token, char *str)
{
	char	*new_str;
	int		i;
	int		j;
	int		total;

	total = ft_strlen(str) + ft_strlen(token->value);
	new_str = ft_malloc(sizeof(char) * (total + 1), 0);
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
	token->value = parsing_strdup(new_str);
}

static void	join_all(t_token **tokens_ptr)
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
			if (ft_strlen(tokens->value) != 0)
			{
				tokens->quoted = tmp->quoted;
			}
			update_token(tokens, to_join_str);
			tokens->is_joignable = tmp->is_joignable;
			if (tmp->type == INFILE || tmp->type == OUTFILE
				|| tmp->type == APPEND_FILE || tmp->type == HEREDOC_DEL)
				tokens->type = tmp->type;
			tokens->next = tmp->next;
		}
		else
			tokens = tokens->next;
	}
}

static void	ignore_all(t_token **tokens)
{
	t_token	*tmp;
	t_token	*last;

	tmp = *tokens;
	last = NULL;
	while (tmp)
	{
		if (tmp->type == IGNORED)
		{
			tmp = tmp->next;
			if (last == NULL)
			{
				*tokens = tmp;
			}
			else
			{
				last->next = tmp;
			}
		}
		else
		{
			last = tmp;
			tmp = tmp->next;
		}
	}
}

static void	clear_tokens(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (ft_strlen(tmp->value) == 0)
		{
			if (tmp->type == HEREDOC_DEL && tmp->quoted != NONE)
				tmp->value = ft_strdup("\0");
			else if (tmp->quoted != NONE)
				tmp->value = ft_strdup("\0");
			else
			{
				tmp->type = IGNORED;
			}
		}
		tmp = tmp->next;
	}
	ignore_all(tokens);
}

// void	print_token(t_token *tokens)
// {
// 	t_token	*tmp;

// 	tmp = tokens;
// 	while (tmp)
// 	{
// 		printf("This tokens is {%s} with type {%d}, ", tmp->value, tmp->type);
// 		printf("and quoted {%d} joignable {%d}\n", tmp->quoted,
// 			tmp->is_joignable);
// 		tmp = tmp->next;
// 	}
// }

t_cmd	*parse_pipe(char *line, char **envp)
{
	t_cmd	*cmds;
	t_token	*tokens;

	cmds = NULL;
	tokens = tokenize(line);
	if (tokens == NULL)
		return (NULL);
	if (validate_pipe(tokens, 1) == -1)
		return (NULL);
	update_tokens(&tokens);
	expand_all(&tokens, envp);
	join_tokens(&tokens);
	join_all(&tokens);
	clear_tokens(&tokens);
	if (tokens && validate_pipe(tokens, 0) == -1)
		return (NULL);
	cmds = build_cmds(tokens);
	if (cmds == NULL && status_fct(-1) == 2)
	{
		ft_malloc(0, 1);
		return (NULL);
	}
	return (cmds);
}
