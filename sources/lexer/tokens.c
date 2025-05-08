/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:53:24 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/21 10:53:46 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdlib.h>

static bool ft_have_a_quote(const char *input, int *i)
{
	int j = (*i);
	while(input[j])
	{
		if (input[j] == '\'' || input[j] == '\"')
			return true;
		else if (input[j] == ' ')
			return false;
		j++;
	}
	return false;
}

static	void	tokenize_next(const char *input, int *i, t_token **token_list)
{
	char			*content;
	token_type		type;

	if (ft_have_a_quote(input, i))
    {
        content = read_quoted((char *)input, i);
        if (!content)
            return;
        ft_add_token(token_list, content, TOKEN_WORD);
        free(content);
    }
	else if (ft_is_operator(input[*i]))
	{
		content = read_operator(input, i);
		type = get_operation_type(content);
		ft_add_token(token_list, content, type);
	}
	else
	{
		content = read_word(input, i);
		ft_add_token(token_list, content, TOKEN_WORD);
		free(content);
	}
}

t_token	*tokenize(const char *input)
{
	t_token	*token_list;
	int		i;

	token_list = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		tokenize_next(input, &i, &token_list);
	}
	return (token_list);
}
