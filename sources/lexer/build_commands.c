/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:46:54 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/21 10:47:14 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void check_and_set_assignment(t_token *token)
{
	char	*equal_sign;

    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            equal_sign = strchr(token->value, '=');
            if (equal_sign && equal_sign != token->value)
            {
                token->type = TOKEN_ASSIGNMENT;
            }
        }
        token = token->next;
    }
}

static t_command *create_command(t_token **tokens)
{
    t_command *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->arg_size = 0;
    cmd->type = TOKEN_WORD;
    cmd->next = NULL;
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        cmd->args = ft_realloc((*tokens)->value, cmd->args);
        if (!cmd->args)
            return (free(cmd), NULL);
        if ((*tokens)->type == TOKEN_WORD)
            check_and_set_assignment(*tokens);
        *tokens = (*tokens)->next;
    }

    if (*tokens && (*tokens)->type == TOKEN_PIPE)
        *tokens = (*tokens)->next;

    return cmd;
}

t_command	*build_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		cmd = create_command(&tokens);
		if (!cmd)
			break ;
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
	}
	set_size(head);
	set_type(head);
	return (head);
}
