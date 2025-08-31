#include "../include/minishell.h"
#include "libft.h"

static int	len_num(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	long	num;

	num = n;
	len = len_num(n);
	result = (char *)ft_malloc(sizeof(char) * (len + 1), 0);
	if (!result)
		return (NULL);
	result[len--] = '\0';
	if (num == 0)
		result[len] = '0';
	if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		result[len--] = num % 10 + '0';
		num /= 10;
	}
	return (result);
}
