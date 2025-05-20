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

int	redirect_in(char **args)
{
	int	fd_in;
	int	fd_out;
	int	i;
	int	ret;

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
		else
			i++;
	}
	return (ret);
}
