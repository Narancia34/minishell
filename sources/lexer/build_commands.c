/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:46:54 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 13:58:00 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int	fill_command_args(t_command *cmd, t_token **tokens)
{
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (!append_arg(cmd, (*tokens)->value))
			return (0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

static	int	handle_quoted_empty_command(t_command *cmd, t_token **tokens)
{
	if (!append_arg(cmd, (*tokens)->value))
		return (0);
	*tokens = (*tokens)->next;
	return (fill_command_args(cmd, tokens));
}

static	void	skip_empty_unquoted_tokens(t_token **tokens)
{
	while (*tokens && (*tokens)->type != TOKEN_PIPE && \
		(!(*tokens)->value || !*((*tokens)->value)) && \
		!((*tokens)->is_single_quoted || (*tokens)->is_double_quoted))
		*tokens = (*tokens)->next;
}

static	t_command	*create_command(t_token **tokens)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->type = TOKEN_WORD;
	cmd->next = NULL;
	if (tokens && (!(*tokens)->value || !*(*tokens)->value))
	{
		if (!handle_quoted_empty_command(cmd, tokens))
			return (free_single_command(cmd), NULL);
		if (*tokens && (*tokens)->type == TOKEN_PIPE)
			*tokens = (*tokens)->next;
		return (cmd);
	}
	skip_empty_unquoted_tokens(tokens);
	if (!fill_command_args(cmd, tokens))
		return (free_single_command(cmd), NULL);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
		*tokens = (*tokens)->next;
	return (cmd);
}

t_command	*build_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;
	int			flag;

	head = NULL;
	current = NULL;
	if (tokens)
		flag = tokens->flag;
	else
		flag = 1;
	while (tokens)
	{
		cmd = create_command(&tokens);
		if (!cmd)
			return (free_cmd(head), NULL);
		cmd->flag = flag;
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
	}
	return (head);
}
