#include "../minishell.h"

char *find_path(char *cmd, char **envp)
{
    struct stat st;
    char **envp_paths;
    char *path;
    int i;

    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &st) == 0 && !S_ISDIR(st.st_mode) && access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        return NULL;
    }
    char *curr_path = ft_strjoin("./", cmd);
    if (stat(curr_path, &st) == 0 && !S_ISDIR(st.st_mode)
        && access(curr_path, X_OK) == 0)
        return curr_path;
    free(curr_path);
    i = 0;
    while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp[i])
        return NULL;

    envp_paths = ft_split(envp[i] + 5, ':');
    if (!envp_paths)
        return NULL;

    path = NULL;
	i = 0;
    while(envp_paths[++i])
    {
        char *part_path = ft_strjoin(envp_paths[i], "/");
        char *full_path = ft_strjoin(part_path, cmd);
        free(part_path);

        if (full_path && access(full_path, X_OK) == 0)
        {
            path = full_path;
            break;
        }
        free(full_path);
    }
    free_all(envp_paths);
    return path;
}

