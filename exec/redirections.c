/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:02:58 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 22:05:25 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	red_out(char **args, t_redirection *fd)
{
	fd->fd_out = open(args[fd->i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->fd_out == -1)
	{
		if (access(args[fd->i + 1], W_OK) != 0)
		{
			ft_putstr_fd(args[fd->i + 1], 2);
			ft_putstr_fd(" :permission denied", 2);
		}
		return (1);
	}
	dup2(fd->fd_out, STDOUT_FILENO);
	close(fd->fd_out);
	fd->i += 2;
	return (0);
}

int	red_inp(char **args, t_redirection *fd)
{
	fd->fd_in = open(args[fd->i + 1], O_RDONLY);
	if (fd->fd_in == -1)
	{
		if (access(args[fd->i + 1], F_OK) != 0)
		{
			ft_putstr_fd(args[fd->i + 1], 2);
			ft_putstr_fd("no such file or directory\n", 2);
		}
		else if (access(args[fd->i + 1], R_OK) != 0)
		{
			ft_putstr_fd(args[fd->i + 1], 2);
			ft_putstr_fd("permission denied\n", 2);
		}
		return (1);
	}
	dup2(fd->fd_in, STDIN_FILENO);
	close(fd->fd_in);
	fd->i += 2;
	return (0);
}

int	red_append(char **args, t_redirection *fd)
{
	fd->fd_out = open(args[fd->i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd->fd_out == -1)
	{
		if (access(args[fd->i + 1], W_OK) != 0)
		{
			ft_putstr_fd(args[fd->i + 1], 2);
			ft_putstr_fd(" :permission denied", 2);
		}
		return (1);
	}
	dup2(fd->fd_out, STDOUT_FILENO);
	close(fd->fd_out);
	fd->i += 2;
	return (0);
}

int	redirect_utils(char **args, t_redirection *fd, t_here_docs **here_docs)
{
	if (ft_strcmp(args[fd->i], ">>") == 0)
	{
		if (red_append(args, fd) == 1)
			return (1);
	}
	else if (ft_strcmp(args[fd->i], "<<") == 0)
	{
		read_from_heredoc(*here_docs);
		*here_docs = (*here_docs)->next;
		fd->i += 2;
	}
	else
		fd->i++;
	return (0);
}

int	redirect(char **args, t_here_docs *here_docs)
{
	t_redirection	fd;

	fd.i = 0;
	while (args[fd.i])
	{
		if (ft_strcmp(args[fd.i], ">") == 0)
		{
			if (red_out(args, &fd) == 1)
				return (1);
		}
		else if (ft_strcmp(args[fd.i], "<") == 0)
		{
			if (red_inp(args, &fd) == 1)
				return (1);
		}
		else
		{
			if (redirect_utils(args, &fd, &here_docs) == 1)
				return (1);
		}
	}
	return (0);
}
