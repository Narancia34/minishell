/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:11:27 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/12 12:06:06 by mgamraou         ###   ########.fr       */
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
		if (ft_strcmp(o_args[i], ">") == 0 || ft_strcmp(o_args[i], "<") == 0 || ft_strcmp(o_args[i], ">>") == 0 || ft_strcmp(o_args[i], "<<") == 0)
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
		if ((ft_strcmp(o_args[i], ">") == 0 || ft_strcmp(o_args[i], "<") == 0 || ft_strcmp(o_args[i], ">>") == 0 || ft_strcmp(o_args[i], "<<") == 0) && o_args[i+1])
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

int	has_heredoc(t_command *input)
{
	int	i;
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

int	check_input(t_command *input, t_env **env_list, char **envp, int *exit_s)
{
	t_command	*tmp;
	char	**args;
	t_here_docs	*here_docs;
	int		ret;

	here_docs = here_doc(input, exit_s, *env_list, envp);
	if (has_heredoc(input) == 0 && !here_docs)
	{
		free_here_docs(here_docs);
		return (257);
	}
	if (has_pipe(input) > 1)
		handle_pipeline(input, env_list, envp, exit_s, here_docs);
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
			else if (is_builtin(args[0]) == 1)
			{
				ret = exec_builtin(args, env_list, tmp->args, exit_s, here_docs);
				if (ret != 257)
				{
					free_here_docs(here_docs);
					clean_up(NULL, args);
					return (ret);
				}
			}
			else
				exec_cmd(args, envp, tmp->args, 0, exit_s, env_list, input, here_docs);
			clean_up(NULL, args);
			tmp = tmp->next;
		}
	}
	if (here_docs)
		free_here_docs(here_docs);
	return (257);
}
