#include "../minishell.h"


void sigquit_handler_parent(int signo)
{
    (void)signo;
}

void sigint_heredoc_handler(int signo)
{
    (void)signo;
    g_last_status = 130;
    write(STDOUT_FILENO, "\n", 1);
    rl_done = 1;
}
void	sigquit_handler(int sig)
{
	(void)sig;
	if (g_last_status == 1)
	{
		write(2, "Quit (core dumped)\n", 19);
	}
}

void sigint_handler_parent(int signo)
{
    (void)signo;
    status_fct(130);
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

int setup_interactive_signals(void)
{
    struct sigaction sa_int;
    
    sa_int.sa_handler = sigint_handler_parent;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa_int, NULL) < 0)
    {
        perror("sigaction SIGINT");
        return -1;
    }
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) // Ignore CTRL+/
    {
        perror("signal SIGQUIT");
        return -1;
    }
    return 0;
}
int setup_child_signals(void)
{
    if (signal(SIGINT, SIG_DFL) == SIG_ERR)
    {
        perror("signal SIGINT child");
        return -1;
    }
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
    {
        perror("signal SIGQUIT child");
        return -1;
    }
    
    return 0;
}
int setup_execution_signals(void)
{
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
    {
        perror("signal SIGINT execution");
        return -1;
    }
    
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
    {
        perror("signal SIGQUIT execution");
        return -1;
    }
    
    return 0;
}
// int setup_heredoc_signals(void)
// {
//     struct sigaction sa_int;

//     sa_int.sa_handler = sigint_heredoc_handler;
//     sigemptyset(&sa_int.sa_mask);
//     sa_int.sa_flags = 0;
//     if (sigaction(SIGINT, &sa_int, NULL) < 0)
//     {
//         perror("sigaction heredoc SIGINT");
//         return -1;
//     }

//     if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
//     {
//         perror("signal heredoc SIGQUIT");
//         return -1;
//     }
    
//     return 0;
// }

int restore_interactive_signals(void)
{
    return setup_interactive_signals();
}

void handle_child_exit_status(int status)
{
    if (WIFEXITED(status))
    {
        status_fct(WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        if (sig == SIGQUIT)
        {
            write(STDERR_FILENO, "Quit (core dumped)\n", 19);
            status_fct(128 + sig);
        }
        else if (sig == SIGINT)
        {
            status_fct(128 + sig);
        }
        else
        {
            status_fct(128 + sig);
        }
    }
}