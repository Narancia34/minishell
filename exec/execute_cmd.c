/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:17:15 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/22 16:23:13 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_exec(char *path, char **args, char **envp)
{
	if (!path)
	{
		ft_putstr_fd("command not found\n", 2);
		clean_up(path, args);
		exit(127);
	}
	if (execve(path, args, envp) == -1)
	{
		perror("failed to execute command");
		clean_up(path, args);
		exit(126);
	}
}

int	redirect_in(char **args)
{
	int	fd_in;
	int	fd_out;
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			fd_in = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd_in, STDOUT_FILENO);
			close(fd_in);
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			fd_out = open(args[i + 1], O_RDONLY);
			dup2(fd_out, STDIN_FILENO);
			close(fd_out);
		}
		i++;
	}
	return (0);
}

void	exec_cmd(char **args, char **envp, char **o_args)
{
	pid_t	pid;
	char	*cmd_path;

	pid = fork();
	if (pid == 0)
	{
		redirect_in(o_args);
		cmd_path = find_cmd_path(args[0], envp);
		handle_exec(cmd_path, args, envp);
	}
	else
		wait(NULL);
}
