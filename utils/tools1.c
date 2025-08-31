#include "../minishell.h"

int is_numeric(const char *str)
{
	int i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return 0;
		i++;
	}
	return 1;
}

int	is_valid(const char *arg)
{
	int	i;

	i = 1;
	if ((!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
void free_char_array(char **env)
{
    int i;

    i = 0;
    if (!env)
        return;
    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}
void free_list(t_env *list)
{
    t_env   *tmp;

    while (list)
    {
        tmp = list->next;
        free(list->key);
        free(list->value);
        free(list);
        list = tmp;
    }
}

void	advance(t_token **tokens_ptr)
{
	while (*tokens_ptr && (*tokens_ptr)->type != PIPE)
		*tokens_ptr = (*tokens_ptr)->next;
	if (*tokens_ptr && (*tokens_ptr)->type == PIPE)
		*tokens_ptr = (*tokens_ptr)->next;
}