#include "../include/minishell.h"

static int	atlas(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	if (!tmp)
	{
		new = ft_malloc(sizeof(t_env), 0);
		new->key = ft_strdup(key);
		if (value)
			new->value = ft_strdup(value);
		else
			new->value = NULL;
		new->next = NULL;
		*env = new;
		return (1);
	}
	return (0);
}

static void	update_or_add_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	if (atlas(env, key, value))
		return ;
	while (tmp)
	{
		if (ft_strncmp(tmp->key, key, ft_strlen(key)) == 0 && is_equal(tmp->key,
				key))
		{
			if (value)
				tmp->value = ft_strdup(value);
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
	new->next = *env;
	*env = new;
}

void	assign(char **key, char **value, const char *args)
{
	char	*eq;

	eq = ft_strchr(args, '=');
	if (eq)
	{
		*key = ft_substr(args, 0, eq - args);
		*value = ft_strdup(eq + 1);
	}
	else
	{
		*key = ft_strdup(args);
		*value = NULL;
	}
}

static int	export_help(t_env **list, char **args)
{
	char	*key;
	char	*value;
	int		i;
	int		j;

	i = 1;
	j = 0;
	while (args[i])
	{
		assign(&key, &value, args[i]);
		if (!is_valid(key))
		{
			print_error_3(ERROR_2, key, ERROR_2_3);
			j = 1;
			i++;
			continue ;
		}
		update_or_add_env(list, key, value);
		i++;
	}
	return (j);
}

int	export(char **args, char ***env)
{
	t_env	*list;
	int		exit_status;

	exit_status = 0;
	if (args[1] == NULL)
	{
		bubble_sort(*env);
		return (0);
	}
	list = transform_t_c(*env);
	if (export_help(&list, args) == 1)
		exit_status = 1;
	*env = transform_c_t(list);
	if (!*env)
		return (1);
	return (exit_status);
}
