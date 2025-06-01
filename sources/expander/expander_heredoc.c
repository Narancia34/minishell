/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:00:18 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/05/30 14:43:02 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expander_heredoc(char	*input, t_env *env_list)
{
	char		*result = NULL;
	const char	*start;

	start = input;
	while (*start)
	{
		if (*start == '$' && is_valid_var_char(*(start + 1), true))
				handle_env_var(env_list, &result, &start);
		else
			result = append_character_as_is(&start, result);
	}
	return (result);
}
