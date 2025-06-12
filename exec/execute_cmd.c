/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:17:15 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/12 12:00:20 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

void	handle_exec(char *path, char **args, char **envp, t_env **env_list, t_command *input, t_pid *pid_list)
{
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		free_commands(input);
		clean_up(path, args);
		clean_up(NULL, envp);
		free_env(env_list);
		if (pid_list)
			free_pids(pid_list);
		exit(127);
	}
	if (execve(path, args, envp) == -1)
	{
		perror("failed to execute command");
		free_commands(input);
		clean_up(path, args);
		exit(126);
	}
}

void	exec_cmd(char **args, char **envp, char **o_args, int has_pipe, int *exit_s, t_env **env_list, t_command *input, t_here_docs *here_docs)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	(void)has_pipe;
	ignore_signals();
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (redirect_in(o_args, *env_list, here_docs) == 1)
		{
			free_commands(input);
			clean_up(NULL, args);
			clean_up(NULL, envp);
			free_env(env_list);
			free_here_docs(here_docs);
			exit(EXIT_FAILURE);
		}
		cmd_path = find_cmd_path(args[0], envp);
		handle_exec(cmd_path, args, envp, env_list, input, NULL);
		free_here_docs(here_docs);
	}
	else
{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_s = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
		g_signal_flag = WTERMSIG(status);
		if (g_signal_flag == 2)
		{
			*exit_s = 130;
			g_signal_flag = 0;
			printf("\n");
		}
		else if (g_signal_flag == 3)
		{
			*exit_s = 131;
			g_signal_flag = 0;
			printf("Quit (core dumped)\n");
		}
	}
		setup_signals();
	}
}

void	exec_piped_cmd(char **args, char **envp, char **o_args, t_env **env_list, t_command *input, t_pid *pid_list, t_here_docs *here_docs)
{
	char	*cmd_path;

	if (redirect_in(o_args, *env_list, here_docs) == 1)
	{
		clean_up(NULL, args);
		exit(EXIT_FAILURE);
	}
	cmd_path = find_cmd_path(args[0], envp);
	handle_exec(cmd_path, args, envp, env_list, input, pid_list);

}
