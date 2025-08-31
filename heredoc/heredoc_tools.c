#include "../include/minishell.h"

void	close_exit(int fd, int status)
{
	close(fd);
	exit_bis(status);
}

static void	sigint_heredoc_handler2(int signo)
{
	int	fd;

	(void)signo;
	fd = fd_fct(-1);
	if (fd != -1)
	{
		close(fd);
		fd_fct(-2);
	}
	ft_malloc(0, 1);
	exit(STATUS_SIGINT);
}

static void	setup_heredoc_signals2(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_heredoc_handler2;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (signal(SIGINT, sigint_heredoc_handler2) < 0)
	{
		perror("sigaction");
		ft_malloc(0, 1);
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, sigint_heredoc_handler2) == SIG_ERR)
	{
		perror("signal");
		ft_malloc(0, 1);
		exit(EXIT_FAILURE);
	}
}

void	handle_fd_error(int fd)
{
	if (fd < 0)
	{
		perror("open temp file");
		exit_bis(EXIT_FAILURE);
	}
	setup_heredoc_signals2();
	fd_fct(fd);
}

int	fd_fct(int fd)
{
	static int	stored_fd = -1;

	if (fd >= 0)
		stored_fd = fd;
	else if (fd == -2)
		stored_fd = -1;
	return (stored_fd);
}
