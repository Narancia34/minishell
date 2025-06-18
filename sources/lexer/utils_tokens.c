/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:52:53 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/14 10:39:29 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *ft_add_token(t_token **token_list, char *value, token_type type, int double_quote, int single_qoute)
{
        t_token *new_token;
        t_token *current;

        new_token = malloc(sizeof(t_token));
        if (!new_token)
                return (NULL);
        new_token->value = ft_strdup(value);
        if (!new_token->value) {
                free(new_token);
                return (NULL);
        }
		new_token->is_double_quoted = double_quote;
		new_token->is_single_quoted = single_qoute;
        new_token->type = type;
        new_token->next = NULL;
        if (*token_list == NULL)
        {
                *token_list = new_token;
                return (new_token);
        }
        current = *token_list;
        while (current->next != NULL)
                current = current->next;
        current->next = new_token;
        return (new_token);
}

char    *read_operator(const char *str, int *i)
{
	int     start;

	start = *i;
	if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
		*i += 2;
	else
		(*i)++;
	return (ft_strndup(&str[start], *i - start));
}

static char *append_content(char *existing, const char *new_content, int len)
{
    char *tmp;
    char *result;

    if (!new_content || len <= 0)
        return existing;
    tmp = ft_strndup(new_content, len);
    if (!tmp)
        return existing;
    if (existing) {
        result = ft_strjoin(existing, tmp);
        free(existing);
        free(tmp);
        return result;
	}
    else
        return tmp;
}

static char *handle_quoted_section(const char *str, int *i, char *current_content)
{
	char *quoted;
	char *new_content;

	quoted = read_quoted((char *)str, i);
	if (!quoted)
	{
		free(current_content);
		return NULL;
	}
	if (current_content)
	{
		new_content = ft_strjoin(current_content, quoted);
		free(current_content);
		free(quoted);
		return new_content;
	}
	else
		return quoted;
}

static char *handle_unquoted_section(const char *str, int *i, int *start, char *current_content)
{
    while (str[*i] && !ft_whitespace(str[*i]) &&
           !ft_is_operator(str[*i]) &&
           str[*i] != '\'' && str[*i] != '"')
	{
        (*i)++;
	}
	if (*i > *start)
		return append_content(current_content, &str[*start], *i - *start);
	else
		return current_content;
}

char *read_word(const char *str, int *i, int *is_single_quoted, int *is_double_quoted)
{
    int start = *i;
    char *content = NULL;
    *is_single_quoted = 0;
    *is_double_quoted = 0;

    while (str[*i] && !ft_whitespace(str[*i]) && !ft_is_operator(str[*i]))
    {
        if (str[*i] == '\'')
        {
            if (*i == start) *is_single_quoted = 1;
            content = handle_unquoted_section(str, i, &start, content);
            content = handle_quoted_section(str, i, content);
			if (!content)
				return NULL;
            start = *i;
        }
        else if (str[*i] == '"')
        {
            if (*i == start) *is_double_quoted = 1;
            content = handle_unquoted_section(str, i, &start, content);
            content = handle_quoted_section(str, i, content);
			if (!content)
				return NULL;
            start = *i;
        }
        else
            (*i)++;
    }
    content = handle_unquoted_section(str, i, &start, content);
    if (content)
        return content;
    else
        return ft_strdup("");
}

token_type      get_operation_type(const char *op)
{
        if (ft_strcmp(op, ">") == 0)
                return (TOKEN_REDIR_OUT);
        if (ft_strcmp(op, ">>") == 0)
                return (TOKEN_REDIR_APPEND);
        if (ft_strcmp(op, "<") == 0)
                return (TOKEN_REDIR_IN);
        if (ft_strcmp(op, "<<") == 0)
                return (TOKEN_HEREDOC);
        if (ft_strcmp(op, "|") == 0)
                return (TOKEN_PIPE);
        return (TOKEN_WORD);
}
