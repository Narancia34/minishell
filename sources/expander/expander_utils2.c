/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:14:16 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/19 15:17:53 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(t_env *env_list, const char *var_name)
{
	while (env_list)
	{
		if (strcmp(env_list->var_name, var_name) == 0)
			return (env_list->var_value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_var_list(t_var	*var_list, const char	*var_name)
{
	while(var_list)
	{
		if (strcmp(var_list->var_name, var_name) == 0)
			return (var_list->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}

char	*strjoin_and_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*expand_tilde(char *input)
{
	char	*home;
	char	*expanded;

	if (input[0] == '~' && (input[1] == '/' || input[1] == '\0'))
	{
		home = getenv("HOME");
		if (home)
		{
			expanded = malloc(ft_strlen(home) + ft_strlen(input) + 1);
			if (!expanded)
				return (ft_strdup(input));
			strcpy(expanded, home);
			strcat(expanded, input + 1);
			return (expanded);
		}
	}
	return (ft_strdup(input));
}
