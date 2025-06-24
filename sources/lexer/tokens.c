/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:53:24 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 13:58:09 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_add_token(t_token **token_list,
		char *value, t_token_type type, int *quote)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
		return (free(new_token), NULL);
	new_token->is_single_quoted = quote[0];
	new_token->is_double_quoted = quote[1];
	new_token->type = type;
	new_token->next = NULL;
	if (*token_list == NULL)
	{
		*token_list = new_token;
		return (new_token);
	}
	current = *token_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_token;
	return (new_token);
}

t_token_type	get_operation_type(const char *op)
{
	if (ft_strcmp(op, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	if (ft_strcmp(op, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(op, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(op, "|") == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

char	*read_quoted(const char *input, int *i)
{
	char	quote;
	int		start;
	char	*content;

	quote = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return (ft_putstr_fd("minishell: syntax error: unclosed quote\n", \
				STDERR_FILENO), NULL);
	content = ft_strndup(&input[start], *i - start);
	(*i)++;
	return (content);
}

static	int	tokenize_next(const char *input, int *i, t_token **token_list)
{
	t_token_type	type;
	char			*content;
	int				quote[2];

	quote[0] = 0;
	quote[1] = 0;
	content = NULL;
	if (ft_is_operator(input[*i]))
	{
		content = read_operator(input, i);
		if (!content)
			return (-1);
		type = get_operation_type(content);
		if (!ft_add_token(token_list, content, type, quote))
			return (free(content), -1);
	}
	else
	{
		content = read_word(input, i, quote);
		if (!content)
			return (-1);
		if (!ft_add_token(token_list, content, TOKEN_WORD, quote))
			return (free(content), -1);
	}
	return (free(content), 0);
}

t_token	*tokenize(const char *input)
{
	t_token	*token_list;
	int		i;

	token_list = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (tokenize_next(input, &i, &token_list) < 0)
		{
			free_tokens(token_list);
			return (NULL);
		}
	}
	return (token_list);
}
