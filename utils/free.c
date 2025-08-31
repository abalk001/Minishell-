#include "../minishell.h"

static void	free_redirections(t_redirection *redirections)
{
	t_redirection	*tmp;

	while (redirections)
	{
		tmp = redirections;
		redirections = redirections->next;
		free(tmp->filenames);
		free(tmp);
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		free_all(tmp->args);
		free_redirections(tmp->redirections);
		free(tmp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
	free(tokens);
}

void	free_all(char **av)
{
	int	i;

	i = 0;
	if (av == NULL)
		return ;
	while (av[i])
	{
		free(av[i]);
		i++;
	}
	free(av);
}

void	*ft_malloc(size_t size, int to_free)
{
	static t_address	*head;
	t_address		*new_node;
	t_address		*last;
	t_address		*tmp;
	void			*new_ptr;

	if (to_free)
	{
		tmp = head;
		while (tmp)
		{
			head = head->next;
			free(tmp->address);
			free(tmp);
			tmp = head;
		}
		return (NULL);
	}
	last = ft_address_last(head);
	new_ptr = malloc(size);
	if (!new_ptr)
	{
		printf("Tracking allocation failed\n");
		return (NULL);
	}
	new_node = malloc(sizeof(t_address));
	if (!new_node)
	{
		printf("Tracking allocation failed\n");
		free(new_ptr);
		return (NULL);
	}
	new_node->address = new_ptr;
	new_node->next = NULL;
	if (!head)
		head = new_node;
	else
		last->next = new_node;
	return (new_ptr);
}
