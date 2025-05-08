/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 09:56:46 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/09 10:05:03 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	arr_size(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**ft_realloc(char *arg, char **old_arr)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (arr_size(old_arr) + 2));
	i = 0;
	if (old_arr)
	{
		while (old_arr[i])
		{
			new_arr[i] = old_arr[i];
			i++;
		}
	}
	new_arr[i++] = arg;
	new_arr[i] = NULL;
	free(old_arr);
	return (new_arr);
}

void set_size(t_command *head)
{
	while (head)
	{
		if (head->args)
		{
			int count = 0;
			while (head->args[count])
				count++;
			head->arg_size = count;
		}
		else
			head->arg_size = 0;
		head = head->next;
	}
}

void	set_type(t_command *head)
{
	while (head)
	{
		if (head->args != NULL)
		{
			if (ft_strncmp(head->args[0], "|", 2) == 0)
				head->type = TOKEN_PIPE;
			else if (ft_strncmp(head->args[0], ">>", 3) == 0)
				head->type = TOKEN_REDIR_APPEND;
			else if (ft_strncmp(head->args[0], ">", 2) == 0)
				head->type = TOKEN_REDIR_OUT;
			else if (ft_strncmp(head->args[0], "<", 2) == 0)
				head->type = TOKEN_REDIR_IN;
			else
				head->type = TOKEN_WORD;
		}
		head = head->next;
	}
}

t_command *build_commands(t_token *tokens)
{
	t_command *head = NULL;
	t_command *current = NULL;

	while (tokens)
	{
		t_command *cmd = malloc(sizeof(t_command));
		cmd->args = NULL;
		cmd->arg_size = 0;
		cmd->type = TOKEN_WORD;
		cmd->next = NULL;
		while (tokens && tokens->type != TOKEN_PIPE)
		{
			cmd->args = ft_realloc(tokens->value, cmd->args);
			tokens = tokens->next;
		}
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;

		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	set_size(head);
	set_type(head);
	return head;
}
