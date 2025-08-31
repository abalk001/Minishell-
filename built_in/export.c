#include "../minishell.h"

static void	update_or_add_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = ft_strdup(value);
			}
			return ;
		}
		tmp = tmp->next;
	}
	new = ft_malloc(sizeof(t_env), 0);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	if (!new->key || (value && !new->value))
	{
		free(new->key);
		free(new->value);
		return ;
	}
	new->next = *env;
	*env = new;
}

int	export(char **args, char ***env)
{
	t_env	*list;
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	if (args[1] == NULL)
	{
		env_local(*env);
		return (0);
	}
	list = transform_t_c(*env);
	if (!list)
		return (1);
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			key = ft_substr(args[i], 0, eq - args[i]);
			value = ft_strdup(eq + 1);
		}
		else
		{
			key = ft_strdup(args[i]);
			value = NULL;
		}
		if (!is_valid(key))
		{
			// fprintf(stderr, ", key);
			free(key);
			free(value);
			free_list(list);
			return (1);
		}
		update_or_add_env(&list, key, value);
		free(key);
		free(value);
		i++;
	}
	free_char_array(*env);
	*env = transform_c_t(list);
	if (!*env)
	{
		free_list(list);
		return (1);
	}
	free_list(list);
	return (0);

}

char	*extract_var_name(char *start)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	while (start[i] && (ft_isalnum(start[i]) || start[i] == '_'))
		i++;
	var = malloc(sizeof(char) * (i + 1));
	if (!var)
		return (NULL);
	j = 0;
	while (j < i)
	{
		var[j] = start[j];
		j++;
	}
	var[i] = '\0';
	return (var);
}
