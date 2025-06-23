/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:53:32 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 15:07:01 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_node(char **variable)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->var_name = ft_strdup(variable[0]);
	if (!node->var_name)
		return (free(node), NULL);
	if (variable[1])
	{
		node->var_value = ft_strdup(variable[1]);
		if (!node->var_value)
		{
			free(node->var_name);
			return (free(node), NULL);
		}
	}
	else
		node->var_value = NULL;
	node->exported = 1;
	node->flag = 0;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	add_node(t_env **env_list, t_env *new_n)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_n;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_n;
	new_n->prev = current;
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	set_env(t_env **env_list)
{
	char	cwd[1024];
	t_env	*pwd_node;
	t_env	*shlvl_node;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return ;
	pwd_node = malloc(sizeof(t_env));
	pwd_node->var_name = ft_strdup("PWD");
	pwd_node->var_value = ft_strdup(cwd);
	pwd_node->next = NULL;
	shlvl_node = malloc(sizeof(t_env));
	shlvl_node->var_name = ft_strdup("SHLVL");
	shlvl_node->var_value = ft_strdup("0");
	shlvl_node->next = NULL;
	*env_list = pwd_node;
	pwd_node->next = shlvl_node;
}

t_env	*init_env(char	**env)
{
	t_env	*env_list;
	t_env	*new;
	char	**variable;
	int		i;

	env_list = NULL;
	if (!*env)
	{
		set_env(&env_list);
		return (env_list);
	}
	i = 0;
	while (env[i])
	{
		variable = ft_split(env[i], '=');
		new = new_node(variable);
		add_node(&env_list, new);
		free_split(variable);
		i++;
	}
	return (env_list);
}
