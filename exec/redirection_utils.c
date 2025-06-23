/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 22:02:32 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 22:03:24 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	return (0);
}
