#include "../minishell.h"


static void	close_red(int fd, int flag)
{
	if (flag == 1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
	else if (flag == 2)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
            perror("dup2");
			close(fd);
			exit(1);
		}
		close(fd);
	}
}
void	redirections_out(t_cmd *cmd)
{
	t_redirection *tmp;
	int (fd), (flags);

	fd = -1;
    tmp = cmd->redirections;
	while (tmp)
	{
		if (tmp->type == APPEND)
			flags = O_CREAT | O_WRONLY | O_APPEND;
		else
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		if (fd != -1)
				close(fd);
		fd = open(tmp->filenames, flags, 0644);
		if (fd < 0)
		{
			perror(tmp->filenames);
			exit(1);
		}
		tmp = tmp->next;
	}
	if (fd >= 0)
		close_red(fd, 2);
}

void	redirection_in(t_cmd *cmd)
{
	int				fd;
	t_redirection	*tmp;

	tmp = cmd->redirections;
	fd = -1;
	while (tmp)
	{
		if (fd != -1)
			close(fd);
		fd = open(tmp->filenames, O_RDONLY);
		if (fd < 0)
		{
			perror(tmp->filenames);
			exit(1);
		}
		tmp = tmp->next;
	}
	if (fd >= 0)
		close_red(fd, 1);
}

