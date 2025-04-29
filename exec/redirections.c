/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:02:58 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/29 14:22:20 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			fd_in = open(args[i + 1], O_RDONLY);
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		else if (ft_strcmp(args[i], ">>") == 0)
		{
			fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		i++;
	}
	return (0);
}
