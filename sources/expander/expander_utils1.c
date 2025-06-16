/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:12:08 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/19 16:02:36 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool is_valid_var_char(char c, bool first_char)
{
	if (first_char)
		return (isalpha(c) || c == '_');
	return (isalnum(c) || c == '_');
}

static	void	handle_exit_status(int exit_status, char **result, const char **start)
{
	char	*exit_code;

	exit_code = ft_itoa(exit_status);
	*result = strjoin_and_free(*result, exit_code);
	free(exit_code);
	*start += 2;
}

void	handle_env_var(t_env *env_list, char **result, const char **start)
{
	const char	*var_start;
	char		*var_name;
	char		*var_value;

	var_start = ++(*start);
	while (is_valid_var_char(**start, *start == var_start))
		(*start)++;
	var_name = ft_strndup(var_start, *start - var_start);
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		*result = strjoin_and_free(*result, var_value);
	free(var_name);
}

char	*append_character_as_is(const char **start, char *result)
{
	char	temp[2];

	temp[0] = **start;
	temp[1] = '\0';
	result = strjoin_and_free(result, temp);
	(*start)++;
	return result;
}


char *expand_env_vars(char *input, int exit_status, t_env *env_list)
{
    char    *result = NULL;
    const char  *start = input;
    char    quote = '\0';

    while (*start)
    {
        if ((*start == '"' || *start == '\''))
        {
            if (quote == *start)
                quote = '\0';
            else if (!quote)
                quote = *start;
            result = append_character_as_is(&start, result);
        }
        else if (quote != '\'' && *start == '$' && (is_valid_var_char(*(start + 1), true) || *(start + 1) == '?'))
        {
            if (*(start + 1) == '?')
                handle_exit_status(exit_status, &result, &start);
            else
                handle_env_var(env_list, &result, &start);
        }
        else
            result = append_character_as_is(&start, result);
    }
    return result;
}
