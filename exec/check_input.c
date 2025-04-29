/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:11:27 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/29 14:16:00 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_pipe(t_command *input)
{
	t_command *tmp = input;
	int res = 0;
	while (tmp)
	{
		tmp = tmp->next;
		res++;
	}
	return (res);
}

char	**get_cmd(char **o_args)
{
	char	**args;
	int		i;
	int		j;
	int		red;

	if (!o_args)
		return (NULL);
	i = 0;
	red = 0;
	while (o_args[i])
	{
		if (ft_strcmp(o_args[i], ">") == 0 || ft_strcmp(o_args[i], "<") == 0)
			red += 2;
		i++;
	}
	args = malloc((i - red + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	j = 0;
	while (o_args[i])
	{
		if ((ft_strcmp(o_args[i], ">") == 0 || ft_strcmp(o_args[i], "<") == 0 || ft_strcmp(o_args[i], ">>") == 0) && o_args[i+1])
			i++;
		else
		{
			args[j] = ft_strdup(o_args[i]);
			j++;
		}
		i++;
	}
	args[j] = NULL;
	return (args);
}

void	check_input(t_command *input, t_env *env_list, char **envp)
{
	t_command	*tmp;
	char	**args;

	if (has_pipe(input) > 1)
		handle_pipeline(input, env_list, envp);
	else
	{
		tmp = input;
		while (tmp)
		{
			args = get_cmd(tmp->args);
			if (!args)
			{
				ft_putstr_fd("minishell: error parsing command\n", 2);
				tmp = tmp->next;
				continue;
			}
			if (is_builtin(args[0]) == 1)
				exec_builtin(args, env_list, tmp->args);
			else
				exec_cmd(args, envp, tmp->args, 0);
			tmp = tmp->next;
		}
	}
}
