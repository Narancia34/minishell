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

static int is_quoted_empty_token(t_token *token)
{
    return token &&
        (!token->value || !*token->value) &&
        (token->is_single_quoted || token->is_double_quoted);
}

static int append_arg(t_command *cmd, const char *value)
{
    char *arg_dup = ft_strdup(value);
    if (!arg_dup)
        return 0;
    char **tmp_args = ft_realloc(arg_dup, cmd->args);
    if (!tmp_args) {
        free(arg_dup);
        return 0;
    }
    cmd->args = tmp_args;
    cmd->arg_size++;
    return 1;
}

static int fill_command_args(t_command *cmd, t_token **tokens)
{
    while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
        if (!append_arg(cmd, (*tokens)->value))
            return 0;
        *tokens = (*tokens)->next;
    }
    return 1;
}

static int handle_quoted_empty_command(t_command *cmd, t_token **tokens)
{
    if (!append_arg(cmd, (*tokens)->value))
        return 0;
    *tokens = (*tokens)->next;
    return fill_command_args(cmd, tokens);
}

static void skip_empty_unquoted_tokens(t_token **tokens)
{
    while (*tokens && (*tokens)->type != TOKEN_PIPE &&
          (!(*tokens)->value || !*((*tokens)->value)) &&
          !((*tokens)->is_single_quoted || (*tokens)->is_double_quoted))
        *tokens = (*tokens)->next;
}

static t_command *create_command(t_token **tokens)
{
    t_command *cmd;

	cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->arg_size = 0;
    cmd->type = TOKEN_WORD;
    cmd->next = NULL;

    if (is_quoted_empty_token(*tokens))
	{
        if (!handle_quoted_empty_command(cmd, tokens))
            return (free_single_command(cmd), NULL);
        if (*tokens && (*tokens)->type == TOKEN_PIPE)
            *tokens = (*tokens)->next;
        return cmd;
    }
    skip_empty_unquoted_tokens(tokens);
    if (!fill_command_args(cmd, tokens))
        return (free_single_command(cmd), NULL);
    if (*tokens && (*tokens)->type == TOKEN_PIPE)
        *tokens = (*tokens)->next;
    return cmd;
}

t_command *build_commands(t_token *tokens)
{
    t_command *head = NULL;
    t_command *current = NULL;
    t_command *cmd;

    int flag = tokens ? tokens->flag : 1; // Default to 1 if tokens is NULL

    while (tokens)
    {
        cmd = create_command(&tokens);
        if (!cmd) {
            free_cmd(head);
            return NULL;
        }
        cmd->flag = flag; // Set flag for each command
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
