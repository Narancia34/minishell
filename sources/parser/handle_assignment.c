/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_assignment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:59:54 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/24 19:57:47 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_variable_assignment(char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	if (str[i] != '=')
		return (false);
	return (true);
}

t_env	*find_env_var(t_env *env_list, const char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->var_name, var_name) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

int	ft_env_var(t_env **env_list, char **parts)
{
	t_env	*existing;
	t_env	*new;

	existing = find_env_var(*env_list, parts[0]);
	if (existing)
	{
		free(existing->var_value);
		if (parts[1])
			existing->var_value = ft_strdup(parts[1]);
		else
			existing->var_value = NULL;
		return (1);
	}
	else
	{
		new = new_node(parts);
		if (new)
		{
			new->exported = 0;
			add_node(env_list, new);
			return (1);
		}
		else
			return (0);
	}
}

int	handle_assignment(t_env **env_list, const char *assignment)
{
	char	**parts;
	int		result;

	if (!is_variable_assignment((char *)assignment))
		return (0);
	parts = ft_split(assignment, '=');
	if (!parts)
		return (0);
	result = ft_env_var(env_list, parts);
	return (result);
}
