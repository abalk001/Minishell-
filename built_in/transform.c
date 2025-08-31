#include "../minishell.h"

t_env	*transform_t_c(char **envp)
{
	t_env	*head;
	t_env	*tmp;
	int		i;
	char	*eq;
	t_env	*node;

	i = -1;
	head = NULL;
	while (envp[++i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
			continue ;
		node = ft_malloc(sizeof(t_env), 0);
		if (!node)
			return (free_list(head), NULL);
		node->key = ft_substr(envp[i], 0, eq - envp[i]);
		node->value = ft_strdup(eq + 1);
		node->next = NULL;
		if (!node->key || !node->value)
			return (free_list(head), free(node->value), free(node->key),NULL);
		if (!head)
			head = node;
		else
		{
			tmp = head;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = node;
		}
	}
	return (head);
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
	if (!envp)
		return (NULL);
	tmp = env;
	count = 0;
	while (tmp)
	{
		if (tmp->value)
			envp[count] = ft_strjoin3(tmp->key, "=", tmp->value);
		else
			envp[count] = ft_strdup(tmp->key);
		if (!envp[count])
		{
			while (--count >= 0)
				free(envp[count]);
			return (NULL);
		}
		tmp = tmp->next;
		count++;
	}
	envp[count] = NULL;
	return (envp);
}
