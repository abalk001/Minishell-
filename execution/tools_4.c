#include "../include/minishell.h"

static void	gamma(t_cmd *cur, char *path)
{
	struct stat	st;

	if (ft_strlen(cur->args[0]) > 0
		&& cur->args[0][ft_strlen(cur->args[0]) - 1] == '/')
	{
		if (stat(cur->args[0], &st) == 0 && S_ISDIR(st.st_mode))
		{
			print_error_2(cur->args[0], IS_DIR);
			exit_bis(STATUS_ISDIR);
		}
		if (stat(path, &st) == -1)
		{
			print_error_2(cur->args[0], NO_FILE_DIR);
			exit_bis(STATUS_NOTFOUND);
		}
	}
	if (!path)
	{
		if (ft_strlen(cur->args[0]) == 0)
			cur->args[0] = "''";
		print_error_2(cur->args[0], CMD_NOTFOUND);
		exit_bis(STATUS_NOTFOUND);
	}
}

static void	beta(t_cmd *cur, char ***env, char *path)
{
	if (access(path, X_OK) == -1)
	{
		print_error_2(cur->args[0], PERM_DENIED);
		ft_malloc(0, 1);
		exit(STATUS_NOTFOUND);
	}
	if (ft_strlen(cur->args[0]) == 0)
	{
		cur->args[0] = "''";
		print_error_2(cur->args[0], CMD_NOTFOUND);
		exit_bis(STATUS_NOTFOUND);
	}
	if (execve(path, cur->args, *env) == -1)
	{
		perror("execve");
		ft_malloc(0, 1);
		exit(EXIT_FAILURE);
	}
	ft_malloc(0, 1);
	exit(EXIT_SUCCESS);
}

static void	path_bis(t_path_data *p_data, char *cmd)
{
	struct stat	st;

	p_data->i = -1;
	while (p_data->envp_paths[++p_data->i])
	{
		p_data->part_path = ft_strjoin(p_data->envp_paths[p_data->i], "/");
		p_data->full_path = ft_strjoin(p_data->part_path, cmd);
		if (!p_data->full_path)
			continue ;
		if (stat(p_data->full_path, &st) == 0 && !S_ISDIR(st.st_mode))
		{
			p_data->path = p_data->full_path;
			break ;
		}
	}
}

static char	*find_path(char *cmd, char **envp)
{
	t_path_data	p_data;

	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &p_data.st) == 0 && !S_ISDIR(p_data.st.st_mode)
			&& access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	p_data.curr_path = ft_strjoin("./", cmd);
	if (stat(p_data.curr_path, &p_data.st) == 0 && !S_ISDIR(p_data.st.st_mode)
		&& access(p_data.curr_path, X_OK) == 0)
		return (p_data.curr_path);
	p_data.i = 0;
	while (envp[p_data.i] && ft_strncmp(envp[p_data.i], "PATH=", 5) != 0)
		p_data.i++;
	if (!envp[p_data.i])
		return (NULL);
	p_data.envp_paths = ft_split(envp[p_data.i] + 5, ':');
	if (!p_data.envp_paths)
		return (NULL);
	p_data.path = NULL;
	path_bis(&p_data, cmd);
	return (p_data.path);
}

void	alpha(t_cmd *cur, char ***env)
{
	char	*path;
	int		fd;

	if (cur->redirections && cur->redirections->type == HEREDOC)
	{
		fd = open(cur->redirections->filenames, O_RDONLY);
		if (fd < 0)
		{
			perror("open heredoc");
			exit(EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		unlink(cur->redirections->filenames);
		free(cur->redirections->filenames);
		cur->redirections->filenames = NULL;
	}
	path = find_path(cur->args[0], *env);
	gamma(cur, path);
	beta(cur, env, path);
}
