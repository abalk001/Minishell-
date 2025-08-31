#include "../include/minishell.h"

static void	get_total_helper(const char *str, int *i, int *len, char **envp)
{
	char	*key;
	char	*val;

	if (str[*i] == '$' && str[*i + 1]
		&& (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'))
	{
		(*i)++;
		key = extract_var_name((char *)(str + *i));
		if (key)
			val = find_env_value(key, envp);
		else
			val = " ";
		if (val)
			*len += ft_strlen(val);
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		free(key);
	}
	else
	{
		(*len)++;
		(*i)++;
	}
}

int	get_total(const char *str, char **envp)
{
	int		i;
	char	*status_str;
	int		len;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			status_str = ft_itoa(status_fct(-1));
			if (status_str)
			{
				len += ft_strlen(status_str);
			}
			i += 2;
			continue ;
		}
		get_total_helper(str, &i, &len, envp);
	}
	return (len);
}

static void	expand_into_helper(const char *str, t_expansion_data *data,
		char *new, char **envp)
{
	char	*key;
	char	*val;

	if (str[(*data).i] == '$' && str[(*data).i + 1]
		&& (ft_isalnum(str[(*data).i + 1]) || str[(*data).i + 1] == '_'))
	{
		((*data).i)++;
		key = extract_var_name((char *)(str + (*data).i));
		if (key)
			val = find_env_value(key, envp);
		else
			val = NULL;
		if (val)
		{
			(*data).k = 0;
			while (val[(*data).k])
				new[((*data).j)++] = val[((*data).k)++];
		}
		free(key);
		while (str[(*data).i] && (ft_isalnum(str[(*data).i])
				|| str[(*data).i] == '_'))
			((*data).i)++;
	}
	else
		new[((*data).j)++] = str[((*data).i)++];
}

void	expand_into(const char *str, char **envp, char *new, int *to_double)
{
	t_expansion_data	ex_data;

	ex_data.j = 0;
	ex_data.i = 0;
	while (str[ex_data.i])
	{
		if (str[ex_data.i] == '$' && ex_data.i > 0 && str[ex_data.i - 1] == '='
			&& to_double != NULL)
			*to_double = 1;
		if (str[ex_data.i] == '$' && str[ex_data.i + 1] == '?')
		{
			ex_data.status_str = ft_itoa(status_fct(-1));
			if (ex_data.status_str)
			{
				ex_data.k = 0;
				while (ex_data.status_str[ex_data.k])
					new[ex_data.j++] = ex_data.status_str[ex_data.k++];
			}
			ex_data.i += 2;
			continue ;
		}
		expand_into_helper(str, &ex_data, new, envp);
	}
	new[ex_data.j] = '\0';
}

void	expand_all(t_token **tokens_ptr, char **envp)
{
	t_token	*tokens;

	tokens = *tokens_ptr;
	while (tokens)
	{
		if ((tokens->quoted == DOUBLE || tokens->quoted == NONE)
			&& ft_strchr(tokens->value, '$') && tokens->type != HEREDOC_DEL)
		{
			expand_token(tokens, envp);
		}
		tokens = tokens->next;
	}
}
