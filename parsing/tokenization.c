#include "../minishell.h"

static t_token	*new_token(char *value, t_token_type type, t_quoted quoted,
		int joignable)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	if (ft_strlen(value) == 0)
		token->value = parsing_strdup("''");
	else
		token->value = parsing_strdup(value);
	token->type = type;
	token->next = NULL;
	token->quoted = quoted;
	token->is_joignable = joignable;
	return (token);
}

int	is_one_word(char *str)
{
	int		i;
	char	**splitted;
	splitted = ft_split(str, ' ');
	i = 0;
	while (splitted[i])
		i++;
	free_all(splitted);
	if (i > 1)
		return (0);
	return (1);
}

char	*extract(char **line, int *joignable)
{
	char	*start;

	start = *line;
	while (!is_space(**line) && !ft_strchr("<>|\"'", **line) && **line != '\0')
		(*line)++;
	if (!ft_strrchr("<|> ", **line))
		*joignable = 1;
	return (parsing_strndup(start, *line - start));
}

void	prepare_token(t_tokenize_data *data)
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
		free(tok_data.value);
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

	total = get_total(token->value, envp);
	printf("Total is %d\n", total);
	new = malloc(sizeof(char) * (total + 1));
	if (!new)
		return ;
	expand_into(token->value, envp, new);
	free(token->value);
	token->value = parsing_strdup(new);
	free(new);
}
