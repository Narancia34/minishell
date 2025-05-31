/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:51:42 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/23 16:56:11 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

void	handle_pipe_util_a(int prev_fd, int *fd, t_command *tmp)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (tmp->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

void	handle_pipe_util_b(int *prev_fd, int*fd)
{
	if ((*prev_fd) != -1)
		close((*prev_fd));
	close(fd[1]);
	(*prev_fd) = fd[0];
}

void	handle_pipeline(t_command  *input, t_env **env_list, char **envp, int *exit_s)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	t_command	*tmp;
	char	**args;
	t_pid	*pid_list;
	t_pid	*tmp_n;

	prev_fd = -1;
	tmp = input;
	pid_list = NULL;
	while (tmp)
	{
		if (tmp->next && pipe(fd) == -1)
		{
			perror("pipe failed\n");
			exit(EXIT_FAILURE);
		}
		ignore_signals();
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			handle_pipe_util_a(prev_fd, fd, tmp);
			args = get_cmd(tmp->args);
			if (!args)
			{
				perror("minishell: error parsing command!/n");
				tmp = tmp->next;
				continue;
			}
			if (is_builtin(args[0]) == 1)
				exec_builtin(args, env_list, tmp->args, exit_s);
			else
				exec_cmd(args, envp, tmp->args, 1, exit_s, env_list);
			exit(*exit_s);
		}
		else
			handle_pipe_util_b(&prev_fd, fd);
		tmp_n = make_pid_node(pid);
		(void)tmp_n;
		add_pid_node(&pid_list, tmp_n);
		tmp = tmp->next;
	}
	tmp_n = pid_list;
	while (tmp_n)
	{
		waitpid(tmp_n->pid, &status, 0);
		tmp_n = tmp_n->next;
	}
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
