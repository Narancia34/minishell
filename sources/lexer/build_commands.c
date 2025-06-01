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

static void	free_single_command(t_command *cmd)
{
	if (!cmd) return;
	if (cmd->args) {
		for (int i = 0; cmd->args[i]; i++)
			free(cmd->args[i]);
		free(cmd->args);
	}
	free(cmd);
}

void	free_cmd(t_command *cmd)
{
	t_command *tmp;
	while (cmd) {
		tmp = cmd->next;
		free_single_command(cmd);
		cmd = tmp;
	}
}

static t_command *create_command(t_token **tokens)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->arg_size = 0;
    cmd->type = TOKEN_WORD;
    cmd->next = NULL;
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        char *arg_dup = ft_strdup((*tokens)->value);
        if (!arg_dup) {
            free_single_command(cmd);
            return NULL;
        }
        char **tmp_args = ft_realloc(arg_dup, cmd->args);
        if (!tmp_args) {
            free(arg_dup);
            free_single_command(cmd);
            return NULL;
        }
        cmd->args = tmp_args;
        *tokens = (*tokens)->next;
    }
    if (*tokens && (*tokens)->type == TOKEN_PIPE)
        *tokens = (*tokens)->next;
    return cmd;
}

t_command	*build_commands(t_token *tokens)
{
	t_command	*head = NULL;
	t_command	*current = NULL;
	t_command	*cmd;

	while (tokens)
	{
		cmd = create_command(&tokens);
		if (!cmd) {
			free_cmd(head);
			return NULL;
		}
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
	}
	set_size(head);
	set_type(head);
	return head;
}
