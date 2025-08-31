#include "../include/minishell.h"

static void	optimus_prime(t_transform_data *t_data, char **envp)
{
	if (!t_data->eq)
	{
		t_data->node->key = ft_strdup(envp[t_data->i]);
		t_data->node->value = NULL;
	}
	else
	{
		t_data->node->key = ft_substr(envp[t_data->i], 0,
				t_data->eq - envp[t_data->i]);
		t_data->node->value = ft_strdup(t_data->eq + 1);
	}
}

t_env	*transform_t_c(char **envp)
{
	t_transform_data	t_data;

	t_data.i = -1;
	t_data.head = NULL;
	while (envp[++t_data.i])
	{
		t_data.eq = ft_strchr(envp[t_data.i], '=');
		t_data.node = ft_malloc(sizeof(t_env), 0);
		optimus_prime(&t_data, envp);
		t_data.node->next = NULL;
		if (!t_data.head)
			t_data.head = t_data.node;
		else
		{
			t_data.tmp = t_data.head;
			while (t_data.tmp->next)
				t_data.tmp = t_data.tmp->next;
			t_data.tmp->next = t_data.node;
		}
	}
	return (t_data.head);
}

char	**transform_c_t(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**envp;

	tmp = env;
	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = ft_malloc(sizeof(char *) * (count + 1), 0);
	tmp = env;
	count = 0;
	while (tmp)
	{
		if (tmp->value)
			envp[count] = ft_strjoin3(tmp->key, "=", tmp->value);
		else
			envp[count] = ft_strdup(tmp->key);
		tmp = tmp->next;
		count++;
	}
	envp[count] = NULL;
	return (envp);
}
