/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:51:42 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/11 11:33:23 by mgamraou         ###   ########.fr       */
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

int	count_here_docs(char	**cmd)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], "<<") == 0)
			res++;
		i++;
	}
	return (res);
}

void	handle_pipeline(t_command  *input, t_env **env_list, char **envp, int *exit_s, t_here_docs *here_docs)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	t_command	*tmp;
	char	**args;
	t_pid	*pid_list;
	t_pid	*tmp_n;
	t_pid	*to_free;
	int	count;

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
				exec_builtin(args, env_list, tmp->args, exit_s, here_docs);
			else
				exec_piped_cmd(args, envp, tmp->args, env_list, input, pid_list, here_docs);
			clean_up(NULL, args);
			clean_up(NULL, envp);
			free_commands(input);
			free_env(env_list);
			free_pids(pid_list);
			if (here_docs)
				free_here_docs(here_docs);
			exit(*exit_s);
		}
		else
			handle_pipe_util_b(&prev_fd, fd);
		tmp_n = make_pid_node(pid);
		add_pid_node(&pid_list, tmp_n);
		count = count_here_docs(tmp->args);
		while (count > 0)
		{
			here_docs = here_docs->next;
			count--;
		}
		tmp = tmp->next;
	}
	tmp_n = pid_list;
	while (tmp_n)
	{
		waitpid(tmp_n->pid, &status, 0);
		to_free = tmp_n;
		tmp_n = tmp_n->next;
		free(to_free);
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
