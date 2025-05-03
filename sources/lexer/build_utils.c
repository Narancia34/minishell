/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:12:05 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/20 18:12:50 by mlabrirh         ###   ########.fr       */
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

void	set_size(t_command *head)
{
	int	count;

	while (head)
	{
		if (head->args)
		{
			count = 0;
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
