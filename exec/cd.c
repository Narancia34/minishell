/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:07:54 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:17:56 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd_home(t_env *env_list)
{
	t_env	*tmp;
	char	*path;

	path = NULL;
	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp("HOME", tmp->var_name) == 0)
		{
			path = ft_strdup(tmp->var_value);
			break ;
		}
		tmp = tmp->next;
	}
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("chdir");
		return (free(path), 1);
	}
	return (free(path), 0);
}

int	cd_path(char *path)
{
	if (chdir(path) != 0)
	{
		perror("chdir");
		return (1);
	}
	return (0);
}

int	ft_cd(char **arg, t_env **env_list)
{
	char	current_dir[1024];
	char	old_dir[1024];
	char	*path;

	path = NULL;
	getcwd(old_dir, sizeof(old_dir));
	if (arg[1] == NULL)
	{
		if (cd_home(*env_list) == 1)
			return (1);
	}
	else
	{
		if (cd_path(arg[1]) == 1)
			return (1);
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("getcwd()");
		return (1);
	}
	change_pwd_env(old_dir, current_dir, env_list);
	return (0);
}
