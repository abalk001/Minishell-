#include "../include/minishell.h"

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

static int	check_start(int start, char *value)
{
	if (start)
	{
		if (is_pipe(value))
			return (print_error_3(SYNTAX_ERROR, value, "'\n"),
				status_fct(STATUS_OTHER), -1);
	}
	return (0);
}

int	validate_pipe(t_token *tokens, int start)
{
	t_token	*tmp;

	tmp = tokens;
	if (check_start(start, tmp->value) == -1)
		return (-1);
	while (tmp)
	{
		if ((is_ex_special(tmp->value) || is_special(tmp->value)) && tmp->next
			&& (is_ex_special(tmp->next->value) || is_special(tmp->next->value)
				|| is_pipe(tmp->next->value)))
			return (print_error_2(SYNTAX_ERROR, tmp->next->value),
				ft_putstr_fd("'\n", 2), status_fct(STATUS_OTHER), -1);
		if ((is_ex_special(tmp->value) || is_special(tmp->value)) && !tmp->next)
			return (ft_putstr_fd(NEWLINE_ERR, 2), status_fct(STATUS_OTHER), -1);
		if (is_pipe(tmp->value) && !tmp->next)
		{
			if (start)
				return (ft_putstr_fd(NEWLINE_ERR, 2), status_fct(STATUS_OTHER),
					-1);
			return (status_fct(0), -1);
		}
		tmp = tmp->next;
	}
	return (0);
}
