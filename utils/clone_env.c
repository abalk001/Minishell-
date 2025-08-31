
#include "../minishell.h"



char **clone_env(char **envp)
{
    int i = 0;
    int len = 0;
    char **clone;

    while (envp[len])
        len++;

    clone = ft_malloc((len + 1) * sizeof(char *), 0);
    if (!clone)
        return (NULL);

    while (i < len)
    {
        clone[i] = ft_strdup(envp[i]);
        if (!clone[i])
        {
            while (--i >= 0)
                free(clone[i]);
            return (NULL);
        }
        i++;
    }
    clone[i] = NULL;
    return (clone);
}