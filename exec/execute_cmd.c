/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:17:15 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/19 17:49:22 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

void	handle_exec(char *path, char **args, char **envp, t_env **env_list, t_command *input)
{
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		free_commands(input);
		clean_up(path, args);
		clean_up(NULL, envp);
		t_env *tmp;
		while (*env_list)
		{
			free((*env_list)->var_name);
			free((*env_list)->var_value);
			tmp = *env_list;
			*env_list = (*env_list)->next;
			free(tmp);
		}
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

void	exec_cmd(char **args, char **envp, char **o_args, int has_pipe, int *exit_s, t_env **env_list, t_command *input)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;

	if (has_pipe == 0)
	{
		ignore_signals();
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (redirect_in(o_args) == 1)
			{
				clean_up(NULL, args);
				exit(EXIT_FAILURE);
			}
			cmd_path = find_cmd_path(args[0], envp);
			handle_exec(cmd_path, args, envp, env_list, input);
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
	else
{
		 if (redirect_in(o_args) == 1)
		{
			clean_up(NULL, args);
			exit(EXIT_FAILURE);
		}
		cmd_path = find_cmd_path(args[0], envp);
		handle_exec(cmd_path, args, envp, env_list, input);
	}
}
