/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:09:24 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/19 15:10:41 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

void expand_tokens(t_token *tokens, int exit_status, t_env *env_list)
{
	t_token *tok;
	char	*expanded;

	tok = tokens;
    while (tok)
	{
        if (tok->type == TOKEN_WORD && !tok->is_single_quoted)
		{
            expanded = expand_env_vars(tok->value, exit_status, env_list);
			if (!expanded)
				expanded = ft_strdup("");
            free(tok->value);
            tok->value = expanded;
        }
		tok = tok->next;
    }
}

char	*expand_input(char *input, int exit_status, t_env *env_list)
{
	char	*expanded_vars;

	if (!input)
		return (NULL);
	expanded_vars =	expand_env_vars(input, exit_status, env_list);
	return (expanded_vars);
}
