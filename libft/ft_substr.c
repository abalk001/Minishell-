#include "libft.h"
#include "../include/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (start + len > s_len)
		len = s_len - start;
	sub = (char *)ft_malloc(len + 1, 0);
	ft_strlcpy(sub, s + start, len + 1);
	return (sub);
}
