/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:05:26 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 16:58:27 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	is_builtin(char *arg)
{
	int		i;
	char	*builtins[8];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	if (!arg)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (strcmp(arg, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	wich_builtin(t_shell *shell, char **arg)
{
	if (ft_strcmp(arg[0], "echo") == 0)
		shell->exit_s = ft_echo(arg);
	if (ft_strcmp(arg[0], "cd") == 0)
		shell->exit_s = ft_cd(arg, &shell->env_list);
	if (ft_strcmp(arg[0], "pwd") == 0)
		shell->exit_s = ft_pwd();
	if (ft_strcmp("env", arg[0]) == 0)
		shell->exit_s = ft_env(shell->env_list);
	if (ft_strcmp(arg[0], "unset") == 0)
		shell->exit_s = ft_unset(arg, &shell->env_list);
	if (ft_strcmp(arg[0], "export") == 0)
		shell->exit_s = ft_export(arg, &shell->env_list);
}

void	save_out_in(int flag)
{
	int	saved_stdout;
	int	saved_stdin;

	if (flag == 0)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
	}
	else
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
	}
}

int	exec_builtin(t_shell *shell, char **arg,
		char **o_args, t_here_docs *here_docs)
{
	int	res;

	if (!arg || !arg[0])
		return (0);
	save_out_in(0);
	if (redirect_in(o_args, shell->env_list, here_docs) == 1)
	{
		ft_putstr_fd("minishell: redirection error\n", 2);
		shell->exit_s = 1;
		if (here_docs)
			free_here_docs(here_docs);
		save_out_in(1);
		return (257);
	}
	wich_builtin(shell, arg);
	if (ft_strcmp("exit", arg[0]) == 0)
	{
		res = ft_exit(arg);
		if (res == 257)
			shell->exit_s = 1;
		else
			return (res);
	}
	save_out_in(1);
	return (257);
}
