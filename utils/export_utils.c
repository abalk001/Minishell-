#include "../include/minishell.h"

static void	init_sort_data(int *i, int *j, int *n)
{
	*i = -1;
	*j = 0;
	*n = 0;
}

void	bubble_sort(char **arr)
{
	t_sort_data	s_d;

	s_d.alpha = arr;
	init_sort_data(&s_d.i, &s_d.j, &s_d.n);
	s_d.alpha = clone_array(arr);
	while (s_d.alpha[s_d.n] != NULL)
		s_d.n++;
	while (++s_d.i < s_d.n - 1)
	{
		s_d.j = -1;
		while (++s_d.j < s_d.n - 1 - s_d.i)
		{
			if (ft_strcmp(s_d.alpha[s_d.j], s_d.alpha[s_d.j + 1]) > 0)
			{
				s_d.temp = s_d.alpha[s_d.j];
				s_d.alpha[s_d.j] = s_d.alpha[s_d.j + 1];
				s_d.alpha[s_d.j + 1] = s_d.temp;
			}
		}
	}
	s_d.i = -1;
	while (++s_d.i < s_d.n)
		ft_printf_export(s_d.alpha[s_d.i], 1);
	s_d.alpha = arr;
}
