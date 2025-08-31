#include "libft.h"

int	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	destlen;
	size_t	srclen;

	srclen = ft_strlen(src);
	if (!dst && size == 0)
		return (srclen);
	destlen = ft_strlen(dst);
	if (size <= destlen)
		return (size + srclen);
	i = 0;
	while (src[i] && destlen + i + 1 < size)
	{
		dst[destlen + i] = src[i];
		i++;
	}
	dst[destlen + i] = '\0';
	return (srclen + destlen);
}
