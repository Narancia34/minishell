/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:25:35 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:25:51 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_export(t_export *export, char *arg)
{
	export->i = 0;
	export->flag = 1;
	while (arg[export->i] != '=' && arg[export->i])
		export->i++;
	export->var_name = malloc(sizeof(char) * (export->i + 1));
	if (arg[export->i] == '=')
		export->var_value = ft_strdup(arg + export->i + 1);
	else
		export->var_value = ft_strdup("");
	export->j = 0;
	while (arg[export->j] != '=' && arg[export->j])
	{
		export->var_name[export->j] = arg[export->j];
		export->j++;
	}
	if (arg[export->j] == '=')
		export->flag = 0;
	export->var_name[export->j] = '\0';
	if (!ft_isalpha(export->var_name[0]) && export->var_name[0] != '_')
	{
		free(export->var_name);
		free(export->var_value);
		return (1);
	}
	return (0);
}

int	export_vars(t_export *export, t_env **env_list)
{
	export->tmp = *env_list;
	while (export->tmp)
	{
		if (ft_strcmp(export->var_name, export->tmp->var_name) == 0)
		{
			if (export->flag == 1)
			{
				free(export->var_name);
				free(export->var_value);
				return (0);
			}
			export->tmp->flag = 0;
			free(export->tmp->var_value);
			export->tmp->var_value = ft_strdup(export->var_value);
			free(export->var_name);
			free(export->var_value);
			return (0);
		}
		export->tmp = export->tmp->next;
	}
	return (1);
}

int	add_to_env(t_env **env_list, char *arg)
{
	t_export	export;

	if (init_export(&export, arg) == 1)
		return (1);
	export.i = 1;
	if (export_vars(&export, env_list) == 0)
		return (0);
	while (export.var_name[export.i])
	{
		if (!ft_isalnum(export.var_name[export.i])
			&& export.var_name[export.i] != '_')
		{
			free(export.var_name);
			free(export.var_value);
			return (1);
		}
		export.i++;
	}
	export.new = make_node(export.var_name, export.var_value, export.flag);
	add_to_list(env_list, export.new);
	free(export.var_name);
	free(export.var_value);
	return (0);
}
