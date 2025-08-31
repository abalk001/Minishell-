#include "../minishell.h"

int	count_commands(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		num_cmds;

	num_cmds = 0;
	tmp = cmds;
	while(tmp)
	{
		num_cmds++;
		tmp = tmp->next;
	}
	return (num_cmds);
}

static void handle_all_redirections_of_type(t_cmd *cmd, t_token_type type)
{
    t_redirection *tmp = cmd->redirections;
    int fd = -1;
    int flags;

    while (tmp)
    {
        if (tmp->type == type)
        {
            if (fd != -1)
                close(fd);
            if (type == REDIRECT_IN)
            {
                fd = open(tmp->filenames, O_RDONLY);
                if (fd < 0)
                {
                    perror(tmp->filenames);
                    exit(1);
                }
            }
            else if (type == REDIRECT_OUT)
            {
                flags = O_CREAT | O_WRONLY | O_TRUNC;
                fd = open(tmp->filenames, flags, 0644);
                if (fd < 0)
                {
                    perror(tmp->filenames);
                    exit(1);
                }
            }
            else if (type == APPEND)
            {
                flags = O_CREAT | O_WRONLY | O_APPEND;
                fd = open(tmp->filenames, flags, 0644);
                if (fd < 0)
                {
                    perror(tmp->filenames);
                    exit(1);
                }
            }
        }
        tmp = tmp->next;
    }
    if (fd != -1)
    {
        if (type == REDIRECT_IN)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
        }
        else if (type == REDIRECT_OUT || type == APPEND)
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
        }
        close(fd);
    }
}

void	redirec_here_and_there(t_cmd *cur)
{
    t_redirection *redir;
    int has_heredoc = 0;
    
    if (!cur || !cur->redirections)
        return;
    
    redir = cur->redirections;
    while (redir)
    {
        if (redir->type == HEREDOC)
        {
            has_heredoc = 1;
            break;
        }
        redir = redir->next;
    }
    if (has_heredoc)
    {
        if (heredoc(cur) < 0)
            return;
    }
    handle_all_redirections_of_type(cur, REDIRECT_IN);
    handle_all_redirections_of_type(cur, REDIRECT_OUT);
    handle_all_redirections_of_type(cur, APPEND);
}
