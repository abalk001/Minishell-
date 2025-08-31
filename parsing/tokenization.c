#include "../include/minishell.h"

static t_token	*new_token(char *value, t_token_type type, t_quoted quoted,
		int joignable)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), 0);
	token->value = parsing_strdup(value);
	token->type = type;
	token->next = NULL;
	token->quoted = quoted;
	token->is_joignable = joignable;
	return (token);
}

static char	*extract(char **line, int *joignable)
{
	char	*start;

	start = *line;
	while (!is_space(**line) && !ft_strchr("<>|\"'", **line) && **line != '\0')
		(*line)++;
	if (!ft_strrchr("<|> ", **line))
		*joignable = 1;
	return (parsing_strndup(start, *line - start));
}

static void	prepare_token(t_tokenize_data *data)
{
	if (ft_strncmp((*data).p, ">>", 2) == 0 || ft_strncmp((*data).p, "<<",
			2) == 0)
	{
		(*data).value = parsing_strndup((*data).p, 2);
		(*data).type = get_operator_type((*data).value);
		(*data).p += 2;
	}
	else if (ft_strchr("|<>", *(*data).p))
	{
		(*data).value = parsing_strndup((*data).p, 1);
		(*data).type = get_operator_type((*data).value);
		(*data).p += 1;
	}
	else if (*(*data).p == '\'' || *(*data).p == '"')
		(*data).value = extract_quoted(&(*data).p, &(*data).joignable,
				&(*data).quoted);
	else
		(*data).value = extract(&(*data).p, &(*data).joignable);
}

t_token	*tokenize(char *input)
{
	t_tokenize_data	tok_data;

	tok_data.head = NULL;
	tok_data.last = NULL;
	tok_data.p = input;
	while (*tok_data.p)
	{
		skip_spaces(&tok_data.p);
		if (*tok_data.p == '\0')
			break ;
		tok_data.value = NULL;
		tok_data.type = WORD;
		tok_data.quoted = NONE;
		tok_data.joignable = 0;
		prepare_token(&tok_data);
		tok_data.token = new_token(tok_data.value, tok_data.type,
				tok_data.quoted, tok_data.joignable);
		if (!tok_data.head)
			tok_data.head = tok_data.token;
		else
			tok_data.last->next = tok_data.token;
		tok_data.last = tok_data.token;
	}
	return (tok_data.head);
}

void	expand_token(t_token *token, char **envp)
{
	int		total;
	char	*new;
	int		to_double;

	total = get_total(token->value, envp);
	to_double = 0;
	new = ft_malloc(sizeof(char) * (total + 1), 0);
	expand_into(token->value, envp, new, &to_double);
	if (to_double)
	{
		token->quoted = DOUBLE;
	}
	token->value = parsing_strdup(new);
}
