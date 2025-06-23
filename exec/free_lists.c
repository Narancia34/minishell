/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lists.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:17:40 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 15:04:05 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_env **env_list)
{
	t_env	*tmp;

	while (*env_list)
	{
		free((*env_list)->var_value);
		free((*env_list)->var_name);
		tmp = *env_list;
		*env_list = (*env_list)->next;
		free(tmp);
	}
}

void	free_pids(t_pid *pid_list)
{
	t_pid	*tmp;

	while (pid_list)
	{
		tmp = pid_list;
		pid_list = pid_list->next;
		free(tmp);
	}
}

void	free_here_docs(t_here_docs *here_docs)
{
	t_here_docs	*tmp;

	while (here_docs)
	{
		free(here_docs->file_name);
		tmp = here_docs;
		here_docs = here_docs->next;
		free(tmp);
	}
}
