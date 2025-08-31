
#include "../minishell.h"

static void sigint_heredoc_handler2(int signo)
{
	(void)signo;
	exit(130);
}

static void setup_heredoc_signals2(void)
{
	struct sigaction sa_int;

	sa_int.sa_handler = sigint_heredoc_handler2;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) < 0)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}


static char *generate_temp_filename(void)
{
	int urandom_fd;
	unsigned char random_bytes[8];
	char *temp_name;
	char hex_str[17];
	int i;
	
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
	{
		return NULL;
	}
	
	if (read(urandom_fd, random_bytes, sizeof(random_bytes)) != sizeof(random_bytes))
	{
		close(urandom_fd);
		return NULL;
	}
	close(urandom_fd);
	i = -1;
	while(++i < 8)
	{
		snprintf(&hex_str[i * 2], 3, "%02x", random_bytes[i]);
	}
	hex_str[16] = '\0';
	temp_name = malloc(40);
	if (!temp_name)
		return NULL;
	
	snprintf(temp_name, 40, "/tmp/.minishell_%s", hex_str);
	return temp_name;
}

static int write_heredoc_content(char *delimiter, const char *temp_filename)
{
	char *input;
	int temp_fd;
	
	temp_fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (temp_fd < 0)
	{
		perror("open temp file");
		exit(1);
	}

	setup_heredoc_signals2();
	
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			close(temp_fd);
			exit(0);
		}
		if (ft_strncmp(input, delimiter, ft_strlen(input)) == 0)
		{
			free(input);
			close(temp_fd);
			exit(0);
		}
		if (write(temp_fd, input, ft_strlen(input)) < 0 ||
			write(temp_fd, "\n", 1) < 0)
		{
			perror("write to temp file");
			free(input);
			close(temp_fd);
			unlink(temp_filename);
			exit(1);
		}
		
		free(input);
	}
}
static int process_single_heredoc(	char *delimiter)
{
	pid_t heredoc_pid;
	int status;
	char *temp_filename;
	int temp_fd;
	
	temp_filename = generate_temp_filename();
	if (!temp_filename)
	{
		perror("generate temp filename");
		return -1;
	}
	heredoc_pid = fork();
	if (heredoc_pid == -1)
	{
		perror("fork");
		free(temp_filename);
		return -1;
	}
	else if (heredoc_pid == 0)
	{
		write_heredoc_content(delimiter, temp_filename);
		exit(0);
	}
	else
	{
		waitpid(heredoc_pid, &status, 0);
		
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			status_fct(130);
			unlink(temp_filename);
			free(temp_filename);
			return -1;
		}
		
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			status_fct(WEXITSTATUS(status));
			unlink(temp_filename);
			free(temp_filename);
			return -1;
		}
		temp_fd = open(temp_filename, O_RDONLY);
		if (temp_fd < 0)
		{
			perror(temp_filename);
			unlink(temp_filename);
			free(temp_filename);
			return -1;
		}
		
		if (dup2(temp_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 heredoc");
			close(temp_fd);
			unlink(temp_filename);
			free(temp_filename);
			return -1;
		}
		close(temp_fd);
		unlink(temp_filename);
		free(temp_filename);
		return 0;
	}
}
int heredoc(t_cmd *cmd)
{
	t_redirection *redir;
	int last_stdin = -1;
	
	if (!cmd || !cmd->redirections)
		return 0;
	last_stdin = dup(STDIN_FILENO);
	if (last_stdin < 0)
	{
		perror("dup stdin");
		return -1;
	}
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (redir != cmd->redirections)
			{
				if (dup2(last_stdin, STDIN_FILENO) < 0)
				{
					perror("restore stdin");
					close(last_stdin);
					return -1;
				}
			}
			if (process_single_heredoc(redir->filenames) < 0)
			{
				close(last_stdin);
				return -1; 
			}
		}
		redir = redir->next;
	}
	
	close(last_stdin);
	return 0;
}