/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:26:02 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/11 11:30:17 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_env *env_list, const char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->var_name, var_name) == 0)
			return (env_list->var_value);
		env_list = env_list->next;
	}
	return (NULL);
}

static bool is_valid_var_char(char c, bool first_char)
{
	if (first_char)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

static char *strjoin_and_free(char *s1, char *s2)
{
	char *result;

	if (!s1 && !s2)
		return NULL;
	if (!s1)
		return ft_strdup(s2);
	if (!s2)
		return s1;    
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

static char *expand_tilde(char *input)
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
			return expanded;
		}
	}
	return (ft_strdup(input));
}

static void	ft_exit_status(int exit_status, char **result, char **start)
{
	char	*exit_code;

	exit_code = ft_itoa(exit_status);
	*result = strjoin_and_free(*result, exit_code);
	free(exit_code);
	*start += 2;
}

static void	ft_env_vars(t_env *env_list, char **result, char **start)
{
	char	*var_name;
	char	*var_value;
	char	*var_start;

	var_start = ++(*start);
	while (is_valid_var_char(**start, *start == var_start))
		(*start)++;
	var_name = ft_strndup(var_start, *start - var_start);
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		*result = strjoin_and_free(*result, var_value);
	free(var_name);
}

char *expand_env_vars(char *input, int exit_status, t_env *env_list)
{
	char	*result;
	char	*start;
	char	temp[2];

	if (handle_assignment(&env_list, input))
		return ft_strdup(input);
	start  = input;
	result = ft_strdup("");
	while (*start)
	{
		if (*start == '$' && (is_valid_var_char(*(start + 1), true) || *(start + 1) == '?'))
			if (*(start + 1) == '?')
				ft_exit_status(exit_status, &result, &start);
			else
				ft_env_vars(env_list, &result, &start);
		else
		{
			temp[0] = *start;
			temp[1] = '\0';
			result = strjoin_and_free(result, temp);
			start++;
		}
	}
	return (result);
}

static char *remove_quotes(char *input)
{
	char	*result;
	char	*start;
	char	quote;
	char	temp[2];

	result = ft_strdup("");
	start = input;
	quote = '\0';
	while (*start)
	{
		if ((*start == '\'' || *start == '"') && !quote)
		{
			quote = *start;
			start++;
		}
		else if (*start == quote)
		{
			quote = '\0';
			start++;
		}
		else
		{
			temp[0] = *start;
			temp[1] = '\0';
			result = strjoin_and_free(result, temp);
			start++;
		}
	}
	free(input);
	return (result);
}

char *expand_input(char *input, int exit_status, t_env *env_list)
{
	char	*expanded_tilde;
	char	*expanded_vars;
	char	*no_quotes;

	if (!input)
		return (NULL);
	expanded_tilde = expand_tilde(input);
	expanded_vars = expand_env_vars(expanded_tilde, exit_status, env_list);
	no_quotes = remove_quotes(expanded_vars);
	return (no_quotes);
}
