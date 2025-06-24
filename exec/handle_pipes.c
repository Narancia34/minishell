/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:51:42 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/24 10:04:09 by mgamraou         ###   ########.fr       */
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

void	handle_pipeline(t_shell *shell, t_here_docs *here_docs)
{
	t_pipeline	pipeline;

	pipeline.prev_fd = -1;
	pipeline.tmp = shell->input;
	shell->pid_list = NULL;
	pipeline.saved_head = here_docs;
	while (pipeline.tmp)
	{
		if (pipeline.tmp->next && pipe(pipeline.fd) == -1)
		{
			perror("pipe failed\n");
			exit(EXIT_FAILURE);
		}
		ignore_signals();
		pipeline.pid = fork();
		if (pipeline.pid == 0)
			handle_child_p(shell, here_docs, &pipeline);
		else
			handle_pipe_util_b(&pipeline.prev_fd, pipeline.fd);
		handle_pipe_util_c(shell, &pipeline, &here_docs);
	}
	wait_for_pipes(shell, &pipeline);
	setup_signals();
}
