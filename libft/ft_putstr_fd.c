#include "libft.h"

void	ft_putstr_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

void	ft_printf_export(char *str, int fd)
{
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(str, fd);
	ft_putstr_fd("\n", fd);
}
