/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 10:51:42 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/27 15:52:35 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	handle_pipeline(t_command  *input, t_env *env_list, char **envp)
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	t_command	*tmp;
	char	**args;

	prev_fd = -1;
	tmp = input;
	while (tmp)
	{
		if (tmp->next && pipe(fd) == -1)
		{
			perror("pipe failed\n");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == 0)
		{
			handle_pipe_util_a(prev_fd, fd, tmp);
			args = get_cmd(tmp->args);
			if (!args)
			{
				perror("minishell: error parsing command!/n");
				tmp = tmp->next;
				continue;
			}
			if (is_builtin(args[0]) == 1)
				exec_builtin(args, env_list, tmp->args);
			else
				exec_cmd(args, envp, tmp->args, 1);
		}
		else
			handle_pipe_util_b(&prev_fd, fd);
		tmp = tmp->next;
	}
	while (wait(NULL) > 0)
		;
}
