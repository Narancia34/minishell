/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:17:15 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/13 14:19:33 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

void	handle_exec(char *path, char **args, t_shell *shell)
{
	if (execve(path, args, shell->envp) == -1)
	{
		perror("failed to execute command");
		clean_up(NULL, shell->envp);
		free_commands(shell->input);
		free_env(&shell->env_list);
		if (shell->pid_list)
			free_pids(shell->pid_list);
		clean_up(path, args);
		exit(126);
	}
}

void	exec_cmd(t_shell *shell, char **args, char **o_args, t_here_docs *here_docs)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	ignore_signals();
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (redirect_in(o_args, shell->env_list, here_docs) == 1)
		{
			free_commands(shell->input);
			clean_up(NULL, args);
			clean_up(NULL, shell->envp);
			free_env(&shell->env_list);
			if (here_docs)
				free_here_docs(here_docs);
			exit(EXIT_FAILURE);
		}
		free_here_docs(here_docs);
		cmd_path = find_cmd_path(args[0], shell->envp, &shell->exit_s);
		if (!cmd_path)
		{
			free_commands(shell->input);
			clean_up(cmd_path, args);
			clean_up(NULL, shell->envp);
			free_env(&shell->env_list);
			exit(shell->exit_s);
		}
		handle_exec(cmd_path, args, shell);
	}
	else
{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_s = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
		g_signal_flag = WTERMSIG(status);
		if (g_signal_flag == 2)
		{
			shell->exit_s = 130;
			g_signal_flag = 0;
			printf("\n");
		}
		else if (g_signal_flag == 3)
		{
			shell->exit_s = 131;
			g_signal_flag = 0;
			printf("Quit (core dumped)\n");
		}
	}
		setup_signals();
	}
}

void	exec_piped_cmd(t_shell *shell, char **args, char **o_args, t_here_docs *here_docs)
{
	char	*cmd_path;

	if (redirect_in(o_args, shell->env_list, here_docs) == 1)
	{
		free_commands(shell->input);
		clean_up(NULL, args);
		clean_up(NULL, shell->envp);
		free_env(&shell->env_list);
		free_here_docs(here_docs);
		exit(EXIT_FAILURE);
	}
	free_here_docs(here_docs);
	cmd_path = find_cmd_path(args[0], shell->envp, &shell->exit_s);
	if (!cmd_path)
	{
		free_commands(shell->input);
		clean_up(NULL, args);
		clean_up(NULL, shell->envp);
		free_env(&shell->env_list);
		if (shell->pid_list)
			free_pids(shell->pid_list);
		exit(shell->exit_s);
	}
	handle_exec(cmd_path, args, shell);
}
