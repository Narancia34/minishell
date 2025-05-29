/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_pid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:24:53 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/23 16:47:05 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_pid	*make_pid_node(pid_t pid)
{
	t_pid	*node;
	(void)pid;

	node = (t_pid *)malloc(sizeof(t_pid));
	if (!node)
		return (NULL);
	node->pid = pid;
	node->next = NULL;
	return (node);
}

void	add_pid_node(t_pid **pid_list, t_pid *new_n)
{
	t_pid	*current;

	if (!*pid_list)
	{
		*pid_list = new_n;
		return ;
	}
	current = *pid_list;
	while (current->next)
		current = current->next;
	current->next = new_n;
}
