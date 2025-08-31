#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "./libft/libft.h"
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>


#define STATUS_SIGINT 130
extern int g_last_status;

#define ERROR_1 "minishell: exit: too many arguments\n"
#define ERROR_2 "minishell: export : not a valid identifier\n"
#define ERROR_3 "minishell: unset : not a valid identifier\n"

typedef enum e_token_type {
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	INFILE,
	OUTFILE,
	APPEND_FILE,
	HEREDOC_DEL
} t_token_type;

typedef enum e_quoted {
	SINGLE,
	DOUBLE,
	NONE,
} t_quoted;

typedef struct s_token
{
	char *value;
	t_token_type type;
	t_quoted quoted;
	int is_joignable;
	struct s_token *next;
} t_token;

typedef enum s_build_in
{
	CD,
	PWD,
	ECHO,
	UNSET,
	EXPORT,
	ENV,
	EXIT
}	t_build_in;

typedef struct s_redirection
{
	t_token_type type;
	char *filenames;
	struct s_redirection *next;
}	t_redirection;

typedef struct s_cmd
{
	char **args;
	t_redirection *redirections;
	int		is_builtin;
	struct s_cmd *next;
} t_cmd;

typedef struct s_address
{
	void *address;
	struct s_address *next;
}	t_address;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redirection_data
{
	t_redirection	*head;
	t_redirection	*current;
	t_token			*last_token;
	t_redirection	*new_node;
	int				redirections_len;
	int				i;
} t_redirection_data;

typedef struct s_tokenize_data
{
	t_token			*head;
	t_token			*last;
	char			*p;
	char			*value;
	t_token_type	type;
	t_quoted		quoted;
	t_token			*token;
	int				joignable;
} t_tokenize_data;

typedef struct s_expansion_data
{
	int		j;
	int		k;
	int		i;
	char	*key;
	char	*val;
	char	*status_str;
	int   len;
} t_expansion_data;

typedef struct s_modify_data
{
	char	**splitted;
	int		len;
	int		i;
	t_token	*new_head;
	t_token	*curr;
	t_token	*token;
} t_modify_data;


int		ft_strcmp(char *string1, char *string2);
void	free_all(char **av);

void print_cmd(t_cmd *cmds);

/*	========== PARSING ==========	*/
t_cmd			*parse_pipe(char *line, char **envp);
t_token_type	get_operator_type(const char *str);
char			*extract_quoted(char **str, int *joignable, t_quoted *quoted);
// t_token *new_token(char *value, t_token_type type);
int				validate_pipe(t_token *tokens);
void			update_tokens(t_token **tokens);
t_cmd			*build_cmds(t_token *tokens);
t_redirection	*get_redirections(t_token *tokens);
char			**get_args(t_token *tokens, t_token_type type);
void			advance(t_token **tokens_ptr);
t_token			*tokenize(char *input);
void			skip_spaces(char **str);

/*	========== EXECUTION ==========	*/
int				get_cmds_nbr(t_cmd *cmd);

//build in
int				export(char **args, char ***env);
int				cmd_cd(char **args, char **envp);
int				echo_local(char **args);
int				unset(char **args, char ***env);
int				env_local(char **envp);
int				pwd_local(char **args, char **envp);
int				builtin_exit(char **args, char **envp);
/* FREE */
void			free_cmds(t_cmd *cmd);
void			free_all(char **av);
void			free_tokens(t_token *tokens);
/* TOOLS */
int				ft_strcmp(char *string1, char *string2);
void			*ft_malloc(size_t size, int to_free);
int				is_special(char *str);
int				is_space(char c);
char			*ft_strndup(const char *src, int n);
t_address		*ft_address_last(t_address *lst);

/* signals */
void 			sigint_handler_parent(int signo);
void			sigquit_handler_parent(int signo);
void			sigquit_handler(int sig);
int				status_fct(int exit_code);
void			sigint_heredoc_handler(int signo);
int				setup_interactive_signals(void);
int				setup_child_signals(void);
int				setup_execution_signals(void);
// int				setup_heredoc_signals(void);
int				restore_interactive_signals(void);
void			handle_child_exit_status(int status);
// int export_var(char **args, char **envp);
char			**clone_env(char **envp);
/* execution */
void			execute_pipe(t_cmd *cmds, char ***envp);
void			alpha(t_cmd *cur, char ***env);
void			beta(t_cmd *cur, char ***env, char *path);
void			build_in_pipe(t_cmd *cur, char ***env);
void			close_pipes(int nbr, int (*pipes)[2], pid_t *pid, int flag);
void			redirection_in(t_cmd *cmd);
void			redirections_out(t_cmd *cmd);
void			one_cmd(t_cmd *cmds, char ***env);
void			child_stuff(int (*pipes)[2], t_cmd *cur, char ***env, int i, int n);
void			freud(t_cmd *cmds, pid_t *pids, char ***env, int (*pipes)[2]);
int				count_commands(t_cmd *cmds);
/*Transform */
char			**transform_c_t(t_env *env);
t_env			*transform_t_c(char **envp);
/* tools_1 */
int				is_numeric(const char *str);
int				is_valid(const char *arg);
void			free_char_array(char **env);
void			free_list(t_env *list);
void			redirec_here_and_there(t_cmd *cur);
/*tools_env*/
char			*get_env_value(char *name, char **envp);
/*TOOLS STRING*/
char			*ft_strjoin3(const char *s1, const char *s2, const char *s3);
/*TOOLS exe*/
char			*find_path(char *cmd, char **envp);
void 			expand_all(t_token **tokens_ptr, char **envp);
char			*parsing_strndup(const char *src, int n);
char			*parsing_strdup(const char *src);
/*						HEREDOC				*/
int				heredoc(t_cmd *cmd);
int				is_misquoted(char *line);
int	get_files_nb(t_token *tokens, t_token_type type);
char	*get_files(t_token **tokens_ptr, t_token_type type);
void join_tokens(t_token **tokens);
char	*find_env_value(const char *key, char **envp);
void	expand_token(t_token *token, char **envp);
char	*extract_var_name(char *start);
int	get_total(const char *str, char **envp);
void	expand_into(const char *str, char **envp, char *new);
int	is_one_word(char *str);

#endif
