#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/wait.h>

# define STATUS_SIGINT 130
# define STATUS_NOTFOUND 127
# define STATUS_ISDIR 126
# define STATUS_OTHER 2
# define ERROR_1 "minishell: exit: too many arguments\n"
# define ERROR_2 "minishell: export : `"
# define ERROR_2_3 "': not a valid identifier\n"
# define ALLOC_FAILED "Tracking allocation failed\n"
# define NEWLINE_ERR "bash: syntax error near unexpected token `newline'\n"
# define ERROR_LINE "Syntax Error\n"
# define IS_DIR ": Is a directory\n"
# define NO_FILE_DIR ": No such file or directory\n"
# define CMD_NOTFOUND ": command not found\n"
# define PERM_DENIED ": Permission denied\n"
# define SYNTAX_ERROR "bash: syntax error near unexpected token `"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	INFILE,
	OUTFILE,
	APPEND_FILE,
	HEREDOC_DEL,
	IGNORED
}							t_token_type;

typedef enum e_quoted
{
	SINGLE,
	DOUBLE,
	NONE,
}							t_quoted;

typedef struct s_token
{
	char					*value;
	t_token_type			type;
	t_quoted				quoted;
	int						is_joignable;
	struct s_token			*next;
}							t_token;

typedef struct s_redirection
{
	t_token_type			type;
	char					*filenames;
	int						expanded;
	struct s_redirection	*next;
}							t_redirection;

typedef struct s_cmd
{
	char					**args;
	t_redirection			*redirections;
	int						is_builtin;
	struct s_cmd			*next;
}							t_cmd;

typedef struct s_address
{
	void					*address;
	struct s_address		*next;
}							t_address;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;
}							t_env;

typedef struct s_redirection_data
{
	t_redirection			*head;
	t_redirection			*current;
	t_token					*last_token;
	t_redirection			*new_node;
	int						redirections_len;
	int						i;
}							t_redirection_data;

typedef struct s_tokenize_data
{
	t_token					*head;
	t_token					*last;
	char					*p;
	char					*value;
	t_token_type			type;
	t_quoted				quoted;
	t_token					*token;
	int						joignable;
}							t_tokenize_data;

typedef struct s_expansion_data
{
	int						j;
	int						k;
	int						i;
	char					*key;
	char					*val;
	char					*status_str;
	int						len;
}							t_expansion_data;

typedef struct s_modify_data
{
	char					**splitted;
	int						len;
	int						i;
	t_token					*new_head;
	t_token					*curr;
	t_token					*token;
	int						joignable;
}							t_modify_data;

typedef struct s_path_data
{
	struct stat				st;
	char					**envp_paths;
	char					*path;
	int						i;
	char					*curr_path;
	char					*part_path;
	char					*full_path;
}							t_path_data;
typedef struct s_transform_data
{
	t_env					*head;
	t_env					*tmp;
	int						i;
	char					*eq;
	t_env					*node;
}							t_transform_data;

typedef struct s_heredoc_data
{
	char					*input;
	char					*new;
	int						temp_fd;
}							t_heredoc_data;

typedef struct s_sort_data
{
	char					*temp;
	char					**alpha;
	int						i;
	int						n;
	int						j;
}							t_sort_data;

/*	========== BUILT_IN ==========	*/
int							cmd_cd(char **args, char **envp);
int							echo_local(char **args);
char						*find_env_value(const char *key, char **envp);
int							env_local(char **envp);
int							builtin_exit(char **args);
int							export(char **args, char ***env);
int							pwd_local(char **envp);
t_env						*transform_t_c(char **envp);
char						**transform_c_t(t_env *env);
int							unset(char **args, char ***env);

/*	========== EXECUTION ==========	*/
void						freud(t_cmd *cmds, pid_t *pids, char ***env,
								int (*pipes)[2]);
void						execute_pipe(t_cmd *cmds, char ***envp);
void						close_pipes(int nbr, int (*pipes)[2], pid_t *pid,
								int flag);
void						exit_bis(int status);
void						redirec_here_and_there(t_cmd *cur);
int							redirec_here_and_there_one_cmd(t_cmd *cur);
void						parent_wait(t_cmd *cmds, pid_t *pids);

/*	========== HEREDOC ==========	*/
char						*generate_temp_filename(void);
int							fd_fct(int fd);
int							heredoc(t_cmd *cmd, char **envp);
void						alpha(t_cmd *cur, char ***env);
void						close_exit(int fd, int status);
void						handle_fd_error(int fd);
t_redirection				*get_last_heredoc(t_redirection *redir);

/*	========== PARSING ==========	*/
char						**get_args(t_token *tokens, t_token_type type);
t_cmd						*build_cmds(t_token *tokens);
char						*get_files(t_token **tokens_ptr, t_token_type type);
char						*parsing_strndup(const char *src, int n);
char						*parsing_strdup(const char *src);
t_cmd						*parse_pipe(char *line, char **envp);
t_redirection				*get_redirections(t_token *tokens);
void						update_tokens(t_token **tokens);
void						join_tokens(t_token **tokens);
t_token						*tokenize(char *input);
void						expand_token(t_token *token, char **envp);
void						skip_spaces(char **str);
t_token_type				get_operator_type(const char *str);
char						*extract_quoted(char **str, int *joignable,
								t_quoted *quoted);
int							validate_pipe(t_token *tokens, int start);

/*	========== UTILS ==========	*/
char						**clone_env(char **envp);
char						*extract_var_name(char *start);
char						**clone_array(char **arr);
int							get_total(const char *str, char **envp);
void						expand_into(const char *str, char **envp, char *new,
								int *to_double);
void						expand_all(t_token **tokens_ptr, char **envp);
void						*ft_malloc(size_t size, int to_free);
void						print_error_2(char *error, char *message);
void						print_error_3(char *error, char *message,
								char *extras);
int							count_commands(t_cmd *cmds);
int							is_one_word(char *str);
void						sigint_handler_parent(int signo);
int							setup_child_signals(void);
void						signals_restore_bis(void);
int							is_valid(const char *arg);
char						*ft_strjoin3(const char *s1, const char *s2,
								const char *s3);
int							is_space(char c);
int							is_special(char *str);
int							is_pipe(char *str);
int							is_ex_special(char *str);
int							ft_strcmp(char *string1, char *string2);
void						error_ft(char *filenames);
void						bubble_sort(char **arr);

/*	========== MAIN ==========	*/
int							status_fct(int exit_code);
int							main_helper(char **input);
void						ft_printf_export(char *str, int fd);
int							is_equal(char *str1, char *str2);

#endif
