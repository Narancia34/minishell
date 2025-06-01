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

char *read_quoted(const char *input, int *i)
{
    char    quote;
    int     start;
    char    *content;

    quote = input[*i];
    start = ++(*i);
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
        return NULL;
    }
    content = strndup(&input[start], *i - start);
    (*i)++;
    return content;
}

static int tokenize_next(const char *input, int *i, t_token **token_list)
{
    char *content = NULL;
    token_type type;

    if (ft_is_operator(input[*i]))
    {
        content = read_operator(input, i);
        if (!content)
            return -1;
        type = get_operation_type(content);
        if (!ft_add_token(token_list, content, type)) {
            free(content);
            return -1;
        }
        free(content);
    }
    else
    {
        content = read_word(input, i);
        if (!content)
            return -1;
        if (!ft_add_token(token_list, content, TOKEN_WORD)) {
            free(content);
            return -1;
        }
        free(content);
    }
    return 0;
}

t_token *tokenize(const char *input)
{
    t_token *token_list = NULL;
    int i = 0;

    while (input[i])
    {
        while (input[i] && ft_whitespace(input[i]))
            i++;
        if (!input[i])
            break;
        if (tokenize_next(input, &i, &token_list) < 0)
        {
            free_tokens(token_list);
            return NULL;
        }
    }
    return token_list;
}
