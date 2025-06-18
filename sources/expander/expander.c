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
	t_token *tok = tokens;
	t_token *prev = NULL;
	char *expanded;

	tokens->flag = 1;
	while (tok)
	{
		if (tokens->flag && prev && prev->type == TOKEN_HEREDOC && tok->type == TOKEN_WORD && (tok->is_single_quoted || tok->is_double_quoted))
			tokens->flag = 0;
		if (tok->type == TOKEN_WORD && !tok->is_single_quoted &&
			!(prev && prev->type == TOKEN_HEREDOC))
		{
			expanded = expand_env_vars(tok->value, exit_status, env_list, tokens->flag);
			if (!expanded)
				expanded = ft_strdup("");
			free(tok->value);
			tok->value = expanded;
		}
		prev = tok;
		tok = tok->next;
	}
}
