#include "../include/minishell.h"

static int	ft_atoll_overflow(const char *str, long long *res)
{
	int			sign;
	long long	num;
	int			i;

	i = 0;
	num = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			break ;
		if (sign == 1 && (num > (LLONG_MAX - (str[i] - '0')) / 10))
			return (0);
		if (sign == -1 && (num > (-(LLONG_MIN + (str[i] - '0'))) / 10))
			return (0);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	*res = num * sign;
	return (1);
}

static void	write_error(char *str)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	write(STDERR_FILENO, str, ft_strlen(str));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
}

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	unsigned char	status;
	long long		val;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
		status = status_fct(-1);
	else if (!is_numeric(args[1]))
	{
		write_error(args[1]);
		status = 2;
	}
	else if (args[2])
		return (write(STDERR_FILENO, ERROR_1, 36), 1);
	else
	{
		if (!ft_atoll_overflow(args[1], &val))
			status = 2;
		else
			status = (unsigned char)val;
	}
	rl_clear_history();
	ft_malloc(0, 1);
	exit(status);
}
