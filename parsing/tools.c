#include "../minishell.h"

void	skip_spaces(char **str)
{
	while (**str && is_space(**str))
		(*str)++;
}

t_token_type	get_operator_type(const char *str)
{
	if (ft_strncmp(str, "|", 1) == 0)
		return (PIPE);
	if (ft_strncmp(str, "<", 1) == 0 && ft_strlen(str) == 1)
		return (REDIRECT_IN);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(str, ">", 1) == 0 && ft_strlen(str) == 1)
		return (REDIRECT_OUT);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (APPEND);
	return (WORD);
}

char	*extract_quoted(char **line, int *joignable, t_quoted *quoted_ptr)
{
	char	*start;
	char	quoted;

	quoted = 0;
	if (**line == '"' || **line == '\'')
	{
		quoted = **line;
		if (quoted == '"')
			*quoted_ptr = DOUBLE;
		else if (quoted == '\'')
			*quoted_ptr = SINGLE;
		(*line)++;
	}
	start = *line;
	while (**line && **line != quoted)
		(*line)++;
	(*line)++;
	if (!ft_strrchr("<|> ", **line))
		*joignable = 1;
	return (parsing_strndup(start, *line - start - 1));
}

int	validate_pipe(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->next == NULL && is_special(tmp->value))
			return (printf("Error validation token\n"), status_fct(2), -1);
		if (is_special(tmp->value) && tmp->next && is_special(tmp->next->value))
		{
			if (ft_strncmp(tmp->value, "|", 1) == 0
				&& ft_strncmp(tmp->next->value, "|", 1) == 0)
				return (printf("Error validation token\n"), status_fct(2), -1);
			if (is_special(tmp->value) && ft_strncmp(tmp->next->value, "|",
					1) == 0)
				return (printf("Error validation token\n"), status_fct(2), -1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	is_misquoted(char *line)
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
