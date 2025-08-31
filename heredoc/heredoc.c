#include "../include/minishell.h"

static void	write_heredoc(int temp_fd, char *input, const char *temp_filename,
		char *new)
{
	if (new == NULL)
	{
		if (write(temp_fd, input, ft_strlen(input)) < 0 || write(temp_fd, "\n",
				1) < 0)
		{
			perror("write to temp file");
			free(input);
			close(temp_fd);
			unlink(temp_filename);
			exit_bis(EXIT_FAILURE);
		}
	}
	else
	{
		if (write(temp_fd, new, ft_strlen(new)) < 0 || write(temp_fd, "\n",
				1) < 0)
		{
			perror("write to temp file");
			free(new);
			close(temp_fd);
			unlink(temp_filename);
			exit_bis(EXIT_FAILURE);
		}
	}
}

static void	write_heredoc_content(char *del, int expanded,
		const char *temp_filename, char **envp)
{
	t_heredoc_data	h;

	h.new = NULL;
	h.temp_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	handle_fd_error(h.temp_fd);
	while (1)
	{
		h.input = readline("> ");
		if (!h.input)
			close_exit(h.temp_fd, EXIT_SUCCESS);
		if (ft_strncmp(h.input, del, ft_strlen(del)) == 0 && is_equal(del,
				h.input))
		{
			free(h.input);
			close_exit(h.temp_fd, EXIT_SUCCESS);
		}
		if (expanded)
		{
			h.new = ft_malloc(sizeof(char) * (get_total(h.input, envp) + 1), 0);
			expand_into(h.input, envp, h.new, NULL);
		}
		write_heredoc(h.temp_fd, h.input, temp_filename, h.new);
		free(h.input);
	}
}

static int	process_single_heredoc(char *delimiter, int expanded, char **envp,
		t_redirection *redir)
{
	pid_t	pid;
	int		status;
	char	*temp_filename;

	temp_filename = generate_temp_filename();
	if (!temp_filename)
		return (perror("generate temp filename"), -1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	else if (pid == 0)
	{
		write_heredoc_content(delimiter, expanded, temp_filename, envp);
		ft_malloc(0, 1);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);
		if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			|| (WIFEXITED(status) && WEXITSTATUS(status) != 0))
			return (unlink(temp_filename), -1);
		redir->filenames = temp_filename;
		return (0);
	}
}

static int	handle_redir(t_redirection *redir, char **envp,
		t_redirection *last_heredoc)
{
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (process_single_heredoc(redir->filenames, redir->expanded, envp,
					redir) < 0)
				return (-1);
			if (redir != last_heredoc && redir->filenames)
			{
				unlink(redir->filenames);
				redir->filenames = NULL;
			}
		}
		redir = redir->next;
	}
	return (0);
}

int	heredoc(t_cmd *cmd, char **envp)
{
	t_cmd			*tmp;
	t_redirection	*redir;
	t_redirection	*last_heredoc;

	tmp = cmd;
	while (cmd)
	{
		if (cmd->redirections && cmd->redirections->type == HEREDOC)
		{
			last_heredoc = get_last_heredoc(cmd->redirections);
			redir = cmd->redirections;
			if (handle_redir(redir, envp, last_heredoc) == -1)
				return (-1);
			cmd->redirections->filenames = last_heredoc->filenames;
		}
		cmd = cmd->next;
	}
	cmd = tmp;
	return (0);
}
