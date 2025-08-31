/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abalk <abalk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:51:14 by abalk             #+#    #+#             */
/*   Updated: 2025/08/22 20:00:45 by abalk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"


int g_last_status = 0;

static void handle_eof(int *status_code)
{
    *status_code = STATUS_SIGINT;
    // g_last_status = STATUS_SIGINT;
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "exit\n", 5);
}

static void setup_prompt_signals(void)
{
    struct sigaction sa_int;

    sa_int.sa_handler = sigint_handler_parent;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
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
int status_fct(int exit_code)
{
	static int exit_co = 0;
	int tmp;
	if (exit_code == -1)
		return (exit_co);
	else
	{
		exit_co = exit_code;
		tmp = exit_co;
	}
	return (tmp);
}

// void print_cmd(t_cmd *cmds)
// {
//     t_cmd *tmp;
//     int i;

//     tmp = cmds;
//     while (tmp)
//     {
//         printf("This command has :\n");
//         i = 0;
//         while (tmp->args[i])
//         {
//             printf("\tArgument : {%s}\n", tmp->args[i]);
//             i++;
//         }
//         i = 0;
//         printf("\tRedirectons :\n");
//         while (tmp->redirections)
//         {
//             printf("\t\tFile : {%s} with type {%d}\n", tmp->redirections->filenames, tmp->redirections->type);
//             tmp->redirections = tmp->redirections->next;
//         }
        
//         tmp = tmp->next;
//     }
    
// }
int main(int ac, char ** av, char **o_envp)
{
    (void)ac;
    (void)av;
    t_cmd *cmds;
    char *input;
    const char *primary = "V3 > ";
    char **envp = clone_env(o_envp);
    int status;
    setup_prompt_signals(); 

    status = 0;
    while (1)
    {
        input = readline(primary);
        if (!input)
        {
            handle_eof(&status);
            break;
        }
		if (ft_strncmp(input, "", 1) == 0)
		{
			free(input);
			continue;
		}
        if (*input)
            add_history(input);
		if (is_misquoted(input))
		{
			printf("Syntax Error\n");
			status_fct(2);
			free(input);
			continue;
		} 
        cmds = parse_pipe(input, envp);
        
        free(input);
		if (cmds != NULL)
			execute_pipe(cmds, &envp);
        free_cmds(cmds);
    }
    free_all(envp);
    rl_clear_history();
    return (0);
}
