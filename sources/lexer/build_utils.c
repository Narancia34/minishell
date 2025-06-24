/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:12:05 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 13:58:05 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int	arr_size(char **arr)
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

void	free_single_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}

void	free_cmd(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_single_command(cmd);
		cmd = tmp;
	}
}

int	append_arg(t_command *cmd, const char *value)
{
	char	*arg_dup;
	char	**tmp_args;

	arg_dup = ft_strdup(value);
	if (!arg_dup)
		return (0);
	tmp_args = ft_realloc(arg_dup, cmd->args);
	if (!tmp_args)
	{
		free(arg_dup);
		return (0);
	}
	cmd->args = tmp_args;
	return (1);
}
