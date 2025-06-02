/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:02:58 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/19 17:51:15 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	ft_here_doc(char *delimiter, t_env *env_list)
{
	char	*line;
	char	*res;
	char	*tmp;
	int		fd;
	res = ft_strdup("");
	setup_signals();
	while (1)
	{
		line = readline(">");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file\n" , 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break;
		tmp = line;
		line= ft_strjoin(line, "\n");
		free(tmp);
		tmp = res;
		res = ft_strjoin(res, line);
		free(tmp);
		free(line);
	}
	fd = open("test", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("didnt open\n");
		return 1;
}
	ft_putstr_fd(expander_heredoc(res, env_list), fd);
	free(res);
	close(fd);
	fd = open("test", O_RDONLY, 0644);
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink("test");
	return (0);
}
int	redirect_in(char **args, t_env *env_list)
{
	int	fd_in;
	int	fd_out;
	int	i;
	int	ret;
	int out = dup(STDOUT_FILENO);
	int in = dup(STDIN_FILENO);
	i = 0;
	ret = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
				ret = 1;
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			fd_in = open(args[i + 1], O_RDONLY);
			if (fd_in == -1)
				ret = 1;
			else
			{
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			i += 2;
		}
		else if (ft_strcmp(args[i], ">>") == 0)
		{
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out == -1)
				ret = 1;
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<<") == 0)
		{
			dup2(out, STDOUT_FILENO);
			dup2(in, STDIN_FILENO);
			ft_here_doc(args[i + 1], env_list);
			i+=2;
		}
		else
			i++;
	}
	close(in);
	close(out);
	return (ret);
}
