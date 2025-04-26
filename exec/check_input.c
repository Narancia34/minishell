/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:11:27 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/22 16:23:16 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *arg)
{
	int		i;
	char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};

	if (!arg)
		return 0;
	i = 0;
	while (builtins[i])
	{
		if (strcmp(arg, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	exec_builtin(char **arg, t_env *env_list, char **o_args)
{
	if (!arg || !arg[0])
		return ;
	int saved_stdout = dup(STDOUT_FILENO);
	int saved_stdin = dup(STDIN_FILENO);
	if (redirect_in(o_args) != 0) {
		ft_putstr_fd("minishell: redirection error\n", 2);
		dup2(saved_stdout, STDOUT_FILENO);  // Restore stdout
		close(saved_stdout);
		close(saved_stdin);
		return;
	}
	if (ft_strcmp(arg[0], "echo") == 0)
		ft_echo(arg);
	if (ft_strcmp(arg[0], "cd") == 0)
		ft_cd(arg);
	if (ft_strcmp(arg[0], "pwd") == 0)
		ft_pwd();
	if (ft_strcmp("env", arg[0]) == 0)
		ft_env(env_list);
	if (ft_strcmp("exit", arg[0]) == 0)
		exit(0);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
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
		if ((ft_strcmp(o_args[i], ">") == 0 || ft_strcmp(o_args[i], "<") == 0) && o_args[i+1])
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
			exec_builtin(args, env_list, input->args);
		else
			exec_cmd(args, envp, input->args);
		tmp = tmp->next;
	}
}
