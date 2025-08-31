#include "../include/minishell.h"

static void	update_type(t_token *token, t_token_type type)
{
	if (token->type == WORD)
		token->type = type;
}

void	update_tokens(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == REDIRECT_IN)
			update_type(tmp->next, INFILE);
		else if (tmp->type == REDIRECT_OUT)
			update_type(tmp->next, OUTFILE);
		else if (tmp->type == APPEND)
			update_type(tmp->next, APPEND_FILE);
		else if (tmp->type == HEREDOC)
		{
			update_type(tmp->next, HEREDOC_DEL);
		}
		tmp = tmp->next;
	}
}

static t_token	*add_token(char *value, t_token *next)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token), 0);
	token->value = parsing_strdup(value);
	token->type = WORD;
	token->next = next;
	token->quoted = NONE;
	token->is_joignable = 0;
	return (token);
}

static t_token	*modify_token(t_token *tmp)
{
	t_modify_data (m_data);
	m_data.new_head = NULL;
	m_data.curr = NULL;
	m_data.splitted = ft_split(tmp->value, ' ');
	m_data.len = 0;
	m_data.joignable = 0;
	if (tmp->is_joignable)
		m_data.joignable = 1;
	while (m_data.splitted[m_data.len])
		m_data.len++;
	m_data.i = -1;
	while (++m_data.i < m_data.len)
	{
		m_data.token = add_token(m_data.splitted[m_data.i], NULL);
		if (!m_data.new_head)
			m_data.new_head = m_data.token;
		else
		{
			m_data.curr->next = m_data.token;
			m_data.token->is_joignable = m_data.joignable;
		}
		m_data.curr = m_data.token;
	}
	m_data.curr->next = tmp->next;
	return (m_data.new_head);
}

void	join_tokens(t_token **tokens)
{
	t_token	*tmp;
	t_token	*last;
	t_token	*new_tokens;

	tmp = *tokens;
	last = NULL;
	while (tmp)
	{
		if (tmp->quoted == NONE && !is_one_word(tmp->value))
		{
			new_tokens = modify_token(tmp);
			if (!new_tokens)
				return ;
			if (last == NULL)
				*tokens = new_tokens;
			else
				last->next = new_tokens;
			tmp = new_tokens;
		}
		last = tmp;
		tmp = tmp->next;
	}
}
