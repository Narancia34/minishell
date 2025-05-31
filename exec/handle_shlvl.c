/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:19:48 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/19 14:29:39 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_shlvl(t_env **env_list)
{
	t_env	*current;
	char	*tmp;
	int		shlvl;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, "SHLVL") == 0)
		{
			shlvl = ft_atoi(current->var_value);
			free(current->var_value);
			shlvl++;
			tmp = ft_itoa(shlvl);
			current->var_value = ft_strdup(tmp);
			free(tmp);
		}
		current = current->next;
	}
}
