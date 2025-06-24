/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:17:15 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 21:08:10 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_child(t_shell *shell, char **args,
		char **o_args, t_here_docs *here_docs)
{
	char	*cmd_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (redirect(o_args, here_docs) == 1)
	{
		free_all(shell, args);
		exit(EXIT_FAILURE);
	}
	free_here_docs(here_docs);
	cmd_path = find_cmd_path(args[0], shell->envp, &shell->exit_s);
	if (!cmd_path)
	{
		free_all(shell, args);
		exit(shell->exit_s);
	}
	handle_exec(cmd_path, args, shell);
}

void	collect_exit_s(int status, t_shell *shell)
{
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
}

void	exec_cmd(t_shell *shell, char **args,
		char **o_args, t_here_docs *here_docs)
{
	pid_t	pid;
	int		status;

	ignore_signals();
	pid = fork();
	if (pid == 0)
		handle_child(shell, args, o_args, here_docs);
	else
	{
		waitpid(pid, &status, 0);
		collect_exit_s(status, shell);
		setup_signals();
	}
}
