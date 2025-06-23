/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:10:32 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:10:47 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_node_from_kv(const char *name, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	node->var_name = ft_strdup(name);
	node->var_value = ft_strdup(value);
	node->flag = 0;
	node->exported = 1;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	set_pwd_env(t_env **env_list, char *old_dir,
		char *current_dir, t_pwd *pwd)
{
	t_env	*new_oldpwd;
	t_env	*new_pwd;

	if (!pwd->oldpwd)
	{
		new_oldpwd = new_node_from_kv("OLDPWD", old_dir);
		add_node(env_list, new_oldpwd);
	}
	if (!pwd->pwd)
	{
		new_pwd = new_node_from_kv("PWD", current_dir);
		add_node(env_list, new_pwd);
	}
}

void	change_pwd_env(char *old_dir, char *current_dir, t_env **env_list)
{
	t_env	*current;
	t_pwd	*pwd;

	pwd = malloc(sizeof(t_pwd));
	pwd->pwd = 0;
	pwd->oldpwd = 0;
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, "OLDPWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(old_dir);
			pwd->oldpwd = 1;
		}
		else if (ft_strcmp(current->var_name, "PWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(current_dir);
			pwd->pwd = 1;
		}
		current = current->next;
	}
	set_pwd_env(env_list, old_dir, current_dir, pwd);
	free(pwd);
}
