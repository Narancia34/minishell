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
	t_command	*tmp;
	int		res;

	tmp = input;
	res = 0;
	while (tmp)
	{
		tmp = tmp->next;
		res++;
	}
	return (res);
}

typedef struct s_red
{
	char	**args;
	int		i;
	int		j;
	int		red;
}	t_red;

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

typedef struct s_check
{
	t_command	*tmp;
	char	**args;
	t_here_docs	*here_docs;
	int		ret;
}	t_check;

int	check_heredoc(t_check *check, t_command *input, int *exit_s, t_env *env_list, char **envp)
{
	check->here_docs = here_doc(input, exit_s, env_list, envp);
	if (has_heredoc(input) == 0 && !check->here_docs)
	{
		free_here_docs(check->here_docs);
		return (1);
	}
	return (0);
}

int	exec_input(t_check *check, t_command *input, t_env **env_list, char **envp, int	*exit_s)
{
	if (is_builtin(check->args[0]) == 1)
	{
		check->ret = exec_builtin(check->args, env_list, check->tmp->args, exit_s, check->here_docs);
		if (check->ret != 257)
		{
			free_here_docs(check->here_docs);
			clean_up(NULL, check->args);
			return (check->ret);
		}
	}
	else
		exec_cmd(check->args, envp, check->tmp->args, 0, exit_s, env_list, input, check->here_docs);
	clean_up(NULL, check->args);
	return (257);
}

int	check_input(t_command *input, t_env **env_list, char **envp, int *exit_s)
{
	t_check	check;
	int		ret;
	
	if (check_heredoc(&check, input, exit_s, *env_list, envp) == 1)
		return (257);
	if (has_pipe(input) > 1)
		handle_pipeline(input, env_list, envp, exit_s, check.here_docs);
	else
	{
		check.tmp = input;
		while (check.tmp)
		{
			check.args = get_cmd(check.tmp->args);
			ret = exec_input(&check, input, env_list, envp, exit_s);
			if (ret != 257)
				return (ret);
			check.tmp = check.tmp->next;
		}
	}
	if (check.here_docs)
		free_here_docs(check.here_docs);
	return (257);
}
