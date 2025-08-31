#include "../include/minishell.h"

static void	*free_collector(t_address **head)
{
	t_address	*tmp;

	tmp = *head;
	while (tmp)
	{
		(*head) = (*head)->next;
		free(tmp->address);
		free(tmp);
		tmp = (*head);
	}
	return (NULL);
}

static t_address	*ft_address_last(t_address *lst)
{
	t_address	*temp;

	temp = lst;
	while (temp && temp->next)
	{
		temp = temp->next;
	}
	return (temp);
}

void	*ft_malloc(size_t size, int to_free)
{
	static t_address	*head;
	t_address			*new_node;
	t_address			*last;
	void				*new_ptr;

	if (to_free)
		return (free_collector(&head));
	last = ft_address_last(head);
	new_ptr = malloc(size);
	if (!new_ptr)
		exit_bis(EXIT_FAILURE);
	new_node = malloc(sizeof(t_address));
	if (!new_node)
	{
		free(new_ptr);
		exit_bis(EXIT_FAILURE);
	}
	new_node->address = new_ptr;
	new_node->next = NULL;
	if (!head)
		head = new_node;
	else
		last->next = new_node;
	return (new_ptr);
}
