/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:02:58 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/12 11:46:46 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>

/*char	*make_file_name()*/
/*{*/
/*	char	*res;*/
/*	int		fd;*/
/*	int		i;*/
/*	ssize_t	b;*/
/**/
/*	res = malloc(sizeof(char) * 10);*/
/*	if (!res)*/
/*		return (NULL);*/
/*	fd = open("/dev/urandom", O_RDONLY);*/
/*	b = read(fd, res, 9);*/
/*	if (b == -1)*/
/*		perror("read()");*/
/*	res[9] = '\0';*/
/*	close(fd);*/
/*	i = 0;*/
/*	while (res[i])*/
/*	{*/
/*		res[i] = (res[i] % 26) + 'a';*/
/*		i++;*/
/*	}*/
/*	return (res);*/
/*}*/
/**/
/*int	ft_here_doc(char *delimiter, t_env *env_list)*/
/*{*/
/*	char	*line;*/
/*	char	*res;*/
/*	char	*tmp;*/
/*	char	*file_name;*/
/*	int		fd;*/
/*	pid_t	pid;*/
/*	int	status;*/
/**/
/*	ignore_signals();*/
/*	file_name = make_file_name();*/
/*	pid = fork();*/
/*	if (pid == 0)*/
/*	{*/
/*		signal(SIGQUIT, SIG_IGN);*/
/*		signal(SIGINT, SIG_DFL);*/
/*		res = ft_strdup("");*/
/*		while (1)*/
/*		{*/
/*			line = readline(">");*/
/*			if (!line)*/
/*			{*/
/*				ft_putstr_fd("warning: here-document delimited by end-of-file\n" , 2);*/
/*				break ;*/
/*			}*/
/*			if (ft_strcmp(line, delimiter) == 0)*/
/*				break;*/
/*			tmp = line;*/
/*			line= ft_strjoin(line, "\n");*/
/*			free(tmp);*/
/*			tmp = res;*/
/*			res = ft_strjoin(res, line);*/
/*			free(tmp);*/
/*			free(line);*/
/*		}*/
/*		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);*/
/*		if (fd == -1)*/
/*		{*/
/*			printf("didnt open\n");*/
/*			return 1;*/
/*		}*/
/*		ft_putstr_fd(expander_heredoc(res, env_list), fd);*/
/*		free(res);*/
/*		close(fd);*/
/*		free(file_name);*/
/*		exit(EXIT_SUCCESS);*/
/*	}*/
/*	waitpid(pid, &status, 0);*/
/*	fd = open(file_name, O_RDONLY, 0644);*/
/*	dup2(fd, STDIN_FILENO);*/
/*	close(fd);*/
/*	unlink(file_name);*/
/*	free(file_name);*/
/*	setup_signals();*/
/*	return (0);*/
/*}*/

int	read_from_heredoc(t_here_docs *here_docs)
{
	int	fd;

	fd = open(here_docs->file_name, O_RDONLY, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("couldnt open here-document\n", 2);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink(here_docs->file_name);
	/*free(here_docs->file_name);*/
	return (0);
}

int	redirect_in(char **args, t_env *env_list, t_here_docs *here_docs)
{
	int	fd_in;
	int	fd_out;
	int	i;
	int	ret;
	/*int	out = dup(STDOUT_FILENO);*/
	/*int	in = dup(STDIN_FILENO);*/
	(void)env_list;
	i = 0;
	ret = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
				return (1);
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			fd_in = open(args[i + 1], O_RDONLY);
			if (fd_in == -1)
			{
				return (1);
			}
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
			read_from_heredoc(here_docs);
			here_docs = here_docs->next;
			/*save_fd(2);*/
			/*dup2(in, STDIN_FILENO);*/
			/*ft_here_doc(args[i + 1], env_list);*/
			i+=2;
		}
		else
			i++;
	}
	return (ret);
}
