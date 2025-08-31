#include "../minishell.h"

t_address	*ft_address_last(t_address *lst)
{
	t_address	*temp;

	temp = lst;
	while (temp && temp->next)
	{
		temp = temp->next;
	}
	return (temp);
}

int	is_space(char c)
{
	if (c == ' ')
	{
		return (1);
	}
	return (0);
}

// char	*ft_strndup(const char *src, int n)
// {
// 	char	*dest;
// 	int		size;

// 	size = n;
// 	dest = (char *)ft_malloc((size + 1) * sizeof(char), 0);
// 	ft_memcpy(dest, src, size);
// 	dest[size] = '\0';
// 	return (dest);
// }

int	is_special(char *str)
{
	if (ft_strncmp(str, "<", 1) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(str, ">", 1) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(str, "<<", 2) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(str, ">>", 2) == 0)
	{
		return (1);
	}
	else if (ft_strncmp(str, "|", 1) == 0)
	{
		return (1);
	}
	return (0);
}

int	ft_strcmp(char *string1, char *string2)
{
	size_t	i;
	int		diff;

	i = 0;
	diff = 0;
	while (string1[i] && string2[i])
	{
		diff = (unsigned char)string1[i] - (unsigned char)string2[i];
		if (diff != 0)
			return (diff);
		i++;
	}
	return (diff);
}
