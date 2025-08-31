#include "../minishell.h"

char	*parsing_strndup(const char *src, int n)
{
	char	*dest;
	int		size;

	size = n;
	dest = (char *)malloc((size + 1) * sizeof(char));
	if (dest == NULL)
	{
		return (0);
	}
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}

char	*parsing_strdup(const char *src)
{
	char	*dest;
	int		size;

	size = ft_strlen(src);
	dest = (char *)malloc((size + 1) * sizeof(char));
	if (dest == NULL)
		return (0);
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}
