#include "libft.h"
#include "../minishell.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	int		size;

	size = ft_strlen(src);
	dest = (char *)ft_malloc((size + 1) * sizeof(char), 0);
	if (dest == NULL)
		return (0);
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}
