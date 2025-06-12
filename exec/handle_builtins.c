/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:05:26 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/11 11:19:32 by mgamraou         ###   ########.fr       */
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

int	exec_builtin(char **arg, t_env **env_list, char **o_args, int *exit_s, t_here_docs *here_docs)
{
	if (!arg || !arg[0])
		return (0);
	int saved_stdout = dup(STDOUT_FILENO);
	int saved_stdin = dup(STDIN_FILENO);
	if (redirect_in(o_args, *env_list, here_docs) == 1) {
		ft_putstr_fd("minishell: redirection error\n", 2);
		*exit_s = 1;
		if (here_docs)
			free_here_docs(here_docs);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return (0);
	}
	if (ft_strcmp(arg[0], "echo") == 0)
		*exit_s = ft_echo(arg);
	if (ft_strcmp(arg[0], "cd") == 0)
		*exit_s = ft_cd(arg, env_list);
	if (ft_strcmp(arg[0], "pwd") == 0)
		*exit_s = ft_pwd();
	if (ft_strcmp("env", arg[0]) == 0)
		*exit_s = ft_env(*env_list);
	if (ft_strcmp("exit", arg[0]) == 0)
		return (1);
	if (ft_strcmp(arg[0], "unset") == 0)
		*exit_s = ft_unset(arg, env_list);
	if (ft_strcmp(arg[0], "export") == 0)
		*exit_s = ft_export(arg, env_list);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
	return (0);
}
