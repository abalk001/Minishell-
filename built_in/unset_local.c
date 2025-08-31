#include "../include/minishell.h"

static int	is_valid_unset(const char *arg)
{
	int	i;

	i = 1;
	if ((!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	while (arg[i])
	{
		if ((!ft_isalnum(arg[i]) && arg[i] != '_') || arg[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_key(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strlen(key) == ft_strlen(tmp->key) && ft_strncmp(tmp->key, key,
				ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	unset(char **args, char ***env)
{
	t_env	*list;
	int		i;

	list = transform_t_c(*env);
	i = 1;
	while (args[i])
	{
		if (is_valid_unset(args[i]))
			remove_env_key(&list, args[i]);
		i++;
	}
	*env = transform_c_t(list);
	return (0);
}
