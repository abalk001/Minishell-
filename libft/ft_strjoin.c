#include "libft.h"
#include "../include/minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		total_len;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)ft_malloc(sizeof(char) * (total_len + 1), 0);
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = 0;
	return (result);
}
