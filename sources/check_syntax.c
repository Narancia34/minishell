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

bool validate_syntax(t_token *tokens)
{
    t_token *current;

	current = tokens;
    if (!current)
        return (true);
    while (current)
    {
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->type == TOKEN_PIPE)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
                return (false);
            }
        }
        if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND)
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
                return (false);
            }
        }
        if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
                return (false);
            }
        }
        if ((current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND ||
             current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC) &&
            (!current->next))
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
            return (false);
        }
        current = current->next;
    }
    if (tokens && tokens->type == TOKEN_PIPE)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
        return (false);
    }
    return (true);
}
