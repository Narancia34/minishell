/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:07:34 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/24 12:10:16 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*make_file_name(void)
{
	char	*res;
	int		fd;
	int		i;
	ssize_t	b;

	res = malloc(sizeof(char) * 10);
	if (!res)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	b = read(fd, res, 9);
	if (b == -1)
		perror("read()");
	res[9] = '\0';
	close(fd);
	i = 0;
	while (res[i])
	{
		res[i] = (res[i] % 26) + 'a';
		i++;
	}
	return (res);
}

t_here_docs	*make_heredoc_node(char *file_name)
{
	t_here_docs	*node;

	node = malloc(sizeof(t_here_docs));
	if (!node)
		return (NULL);
	node->file_name = ft_strdup(file_name);
	node->next = NULL;
	return (node);
}

void	add_heredoc_node(t_here_docs **here_docs, t_here_docs *new_n)
{
	t_here_docs	*current;

	if (!*here_docs)
	{
		*here_docs = new_n;
		return ;
	}
	current = *here_docs;
	while (current->next)
		current = current->next;
	current->next = new_n;
}
