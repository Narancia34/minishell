/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:50:38 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/24 11:00:33 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	lstlen(t_env *lst)
{
	int	i;
	
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

char	**upd_env(t_env *env_list)
{
	int		i;
	char	**u_env;
	char	*tmp;

	i = lstlen(env_list);
	u_env = malloc((i + 1) * sizeof(char *));
	i = 0;
	while (env_list)
	{
		tmp = ft_strjoin(env_list->var_name, "=");
		tmp = ft_strjoin(tmp, env_list->var_value);
		u_env[i] = ft_strdup(tmp);
		free(tmp);
		env_list = env_list->next;
		i++;
	}
	u_env[i] = NULL;
	return (u_env);
}
