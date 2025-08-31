#include "../include/minishell.h"

char	*parsing_strndup(const char *src, int n)
{
	char	*dest;
	int		size;

	size = n;
	dest = (char *)ft_malloc((size + 1) * sizeof(char), 0);
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}

char	*parsing_strdup(const char *src)
{
	char	*dest;
	int		size;

	size = ft_strlen(src);
	dest = (char *)ft_malloc((size + 1) * sizeof(char), 0);
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}
