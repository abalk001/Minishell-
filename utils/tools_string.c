#include "../minishell.h"

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*res;

	len1 = strlen(s1);
	len2 = strlen(s2);
	len3 = strlen(s3);
	res = ft_malloc(len1 + len2 + len3 + 1, 0);
	if (!res)
		return NULL;
	memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2);
	memcpy(res + len1 + len2, s3, len3);
	res[len1 + len2 + len3] = '\0';
	return res;
}
