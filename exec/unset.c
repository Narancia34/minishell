/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:21:14 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:23:17 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_env_var(t_env **env_list, char *var_name)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, var_name) == 0)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				*env_list = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->var_name);
			free(current->var_value);
			free(current);
			return ;
		}
		current = current->next;
	}
}

int	ft_unset(char **args, t_env **env_list)
{
	int	i;

	if (!args || !args[0] || !env_list || !*env_list)
		return (1);
	i = 1;
	while (args[i])
	{
		remove_env_var(env_list, args[i]);
		i++;
	}
	return (0);
}
