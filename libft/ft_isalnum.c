#include "libft.h"

int	ft_isalnum(int i)
{
	if (ft_isalpha(i) || ft_isdigit(i))
		return (1);
	return (0);
}
