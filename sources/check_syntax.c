/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:13:03 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/10 10:14:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	check_syntax(t_token *current)
{
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (ft_putstr_fd("minishell: syntax error near\
				unexpected token `|'\n", STDERR_FILENO), false);
		if (current->type == TOKEN_REDIR_OUT || \
			current->type == TOKEN_REDIR_APPEND)
			if (!current->next || current->next->type != TOKEN_WORD)
				return (ft_putstr_fd("minishell: syntax error near\
				unexpected token `newline'\n", STDERR_FILENO), false);
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
			if (!current->next || current->next->type != TOKEN_WORD)
				return (ft_putstr_fd("minishell: syntax error near\
				unexpected token `newline'\n", STDERR_FILENO), false);
		if ((current->type == TOKEN_REDIR_OUT || current->type \
			== TOKEN_REDIR_APPEND || current->type \
			== TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC) && \
			(!current->next))
			return (ft_putstr_fd("minishell: syntax error near\
			unexpected token `newline'\n", STDERR_FILENO), false);
		current = current->next;
	}
	return (true);
}

bool	validate_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current)
		return (true);
	else if (!check_syntax(current))
		return (false);
	else if (tokens && tokens->type == TOKEN_PIPE)
		return (ft_putstr_fd("minishell: syntax error near\
		unexpected token `|'\n", STDERR_FILENO), false);
	return (true);
}
