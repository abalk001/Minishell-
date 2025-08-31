#include "../include/minishell.h"

t_redirection	*get_last_heredoc(t_redirection *redir)
{
	t_redirection	*tmp;
	t_redirection	*last;

	tmp = redir;
	last = NULL;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
			last = tmp;
		tmp = tmp->next;
	}
	return (last);
}

static void	byte_to_hex(unsigned char byte, char *dst)
{
	const char	*hex;

	hex = "0123456789abcdef";
	dst[0] = hex[byte >> 4];
	dst[1] = hex[byte & 0x0F];
}

static int	fill_random_hex(char *hex_str)
{
	int				fd;
	unsigned char	random_bytes[8];
	int				i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (-1);
	if (read(fd, random_bytes, 8) != 8)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	i = 0;
	while (i < 8)
	{
		byte_to_hex(random_bytes[i], &hex_str[i * 2]);
		i++;
	}
	hex_str[16] = '\0';
	return (0);
}

static void	copy_prefix(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

char	*generate_temp_filename(void)
{
	char	hex_str[17];
	char	*temp_name;
	int		i;
	int		j;

	if (fill_random_hex(hex_str) < 0)
		return (NULL);
	temp_name = ft_malloc(40, 0);
	if (!temp_name)
		return (NULL);
	copy_prefix(temp_name, "/tmp/minishell_");
	i = 0;
	j = 16;
	while (i < j)
	{
		temp_name[15 + i] = hex_str[i];
		i++;
	}
	temp_name[15 + i] = '\0';
	return (temp_name);
}
