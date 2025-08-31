#include "../include/minishell.h"

static int	get_cmds_nbr_by_tokens(t_token *tokens)
{
	int	len;

	len = 1;
	while (tokens)
	{
		if (ft_strncmp(tokens->value, "|", 1) == 0 && tokens->type == PIPE)
		{
			len++;
		}
		tokens = tokens->next;
	}
	return (len);
}

static int	is_built_in(char *str)
{
	if (ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) == 2)
		return (1);
	if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	if (ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6)
		return (1);
	if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	if (ft_strncmp(str, "unset", 5) == 0 && ft_strlen(str) == 5)
		return (1);
	if (ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	if (ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	return (0);
}

static void	init_cmd(t_cmd **cmd, t_token *tokens)
{
	(*cmd)->is_builtin = 0;
	(*cmd)->args = get_args(tokens, WORD);
	(*cmd)->redirections = get_redirections(tokens);
	(*cmd)->next = NULL;
}

static void	advance(t_token **tokens_ptr)
{
	while (*tokens_ptr && (*tokens_ptr)->type != PIPE)
		*tokens_ptr = (*tokens_ptr)->next;
	if (*tokens_ptr && (*tokens_ptr)->type == PIPE)
		*tokens_ptr = (*tokens_ptr)->next;
}

t_cmd	*build_cmds(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;
	t_cmd	*new_node;
	int		cmds_nbr;
	int		i;

	i = -1;
	head = NULL;
	current = NULL;
	cmds_nbr = get_cmds_nbr_by_tokens(tokens);
	while (++i < cmds_nbr)
	{
		new_node = ft_malloc(sizeof(t_cmd), 0);
		init_cmd(&new_node, tokens);
		if (new_node->args[0] && is_built_in(new_node->args[0]))
			new_node->is_builtin = 1;
		advance(&tokens);
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
	}
	return (head);
}
