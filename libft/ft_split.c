#include "../include/minishell.h"
#include "libft.h"

static int	count_word(const char *s, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*allocate_word(const char *s, int start, int end)
{
	int		len;
	char	*word;
	int		i;

	len = end - start + 1;
	word = (char *)ft_malloc((len + 1) * sizeof(char), 0);
	i = 0;
	if (!word)
		return (NULL);
	while (start <= end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static char	**allocate(const char *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	start = 0;
	arr = ft_malloc((count_word(s, c) + 1) * sizeof(char *), 0);
	if (!arr)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			start = i;
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
		{
			arr[j++] = allocate_word(s, start, i);
			if (!arr[j - 1])
				return (NULL);
		}
		i++;
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	if (!s)
		return (NULL);
	return (allocate(s, c));
}
