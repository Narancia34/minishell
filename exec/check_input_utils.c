/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:22:23 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 14:27:59 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_pipe(t_command *input)
{
	t_command	*tmp;
	int			res;

	tmp = input;
	res = 0;
	while (tmp)
	{
		tmp = tmp->next;
		res++;
	}
	return (res);
}

int	has_heredoc(t_command *input)
{
	int			i;
	t_command	*tmp;

	tmp = input;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			if (ft_strcmp(tmp->args[i], "<<") == 0)
				return (0);
			i++;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	innit_args(t_red *red, char **o_args)
{
	red->i = 0;
	red->red = 0;
	while (o_args[red->i])
	{
		if (ft_strcmp(o_args[red->i], ">") == 0
			|| ft_strcmp(o_args[red->i], "<") == 0
			|| ft_strcmp(o_args[red->i], ">>") == 0
			|| ft_strcmp(o_args[red->i], "<<") == 0)
			red->red += 2;
		red->i++;
	}
	red->args = malloc((red->i - red->red + 1) * sizeof(char *));
	if (!red->args)
		return (1);
	return (0);
}

char	**get_cmd(char **o_args)
{
	t_red	red;

	if (!o_args)
		return (NULL);
	if (innit_args(&red, o_args) == 1)
		return (NULL);
	red.i = 0;
	red.j = 0;
	while (o_args[red.i])
	{
		if ((ft_strcmp(o_args[red.i], ">") == 0
				|| ft_strcmp(o_args[red.i], "<") == 0
				|| ft_strcmp(o_args[red.i], ">>") == 0
				|| ft_strcmp(o_args[red.i], "<<") == 0)
			&& o_args[red.i + 1])
			red.i++;
		else
		{
			red.args[red.j] = ft_strdup(o_args[red.i]);
			red.j++;
		}
		red.i++;
	}
	red.args[red.j] = NULL;
	return (red.args);
}
