#include "../include/minishell.h"

static void	open_files(int *fd, t_token_type type, char *filenames)
{
	int	flags;

	if (*fd != -1)
		close(*fd);
	if (type == REDIRECT_IN)
	{
		*fd = open(filenames, O_RDONLY);
		if (*fd < 0)
			error_ft(filenames);
	}
	else if (type == REDIRECT_OUT)
	{
		flags = O_CREAT | O_WRONLY | O_TRUNC;
		*fd = open(filenames, flags, 0644);
		if (*fd < 0)
			error_ft(filenames);
	}
	else if (type == APPEND)
	{
		flags = O_CREAT | O_WRONLY | O_APPEND;
		*fd = open(filenames, flags, 0644);
		if (*fd < 0)
			error_ft(filenames);
	}
}

static int	handle_redir_bis(t_cmd *cmd, t_token_type type)
{
	t_redirection	*tmp_red;
	int				fd;

	tmp_red = cmd->redirections;
	fd = -1;
	while (tmp_red)
	{
		if (tmp_red->type == type)
		{
			open_files(&fd, type, tmp_red->filenames);
		}
		tmp_red = tmp_red->next;
	}
	return (fd);
}

static void	close_update_status(int fd, int status)
{
	close(fd);
	status_fct(status);
}

static void	handle_all_redirections_of_type(t_cmd *cmd, t_token_type type)
{
	int	fd;

	fd = -1;
	fd = handle_redir_bis(cmd, type);
	if (fd != -1)
	{
		if (type == REDIRECT_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close_update_status(fd, EXIT_FAILURE);
			}
		}
		else if (type == REDIRECT_OUT || type == APPEND)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close_update_status(fd, EXIT_FAILURE);
			}
		}
		close(fd);
	}
}

int	redirec_here_and_there_one_cmd(t_cmd *cur)
{
	handle_all_redirections_of_type(cur, REDIRECT_IN);
	handle_all_redirections_of_type(cur, REDIRECT_OUT);
	handle_all_redirections_of_type(cur, APPEND);
	return (status_fct(-1));
}
