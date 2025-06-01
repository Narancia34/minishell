/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:52:53 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/21 10:53:08 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*ft_add_token(t_token **token_list, char *value, token_type type)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value) {
		free(new_token);
		return (NULL);
	}
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

char	*read_operator(const char *str, int *i)
{
	int	start;

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
    if (!tmp) {
        free(existing);
        return NULL;
    }
    if (existing)
	{
        result = ft_strjoin(existing, tmp);
        free(existing);
        free(tmp);
        if (!result) return NULL;
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
    if (!quoted) {
        free(current_content);
		return NULL;
    }
	if (current_content)
	{
        new_content = ft_strjoin(current_content, quoted);
        free(current_content);
        free(quoted);
        if (!new_content) return NULL;
        return new_content;
	}
	else
        return quoted;
}

static char *handle_unquoted_section(const char *str, int *i, int *start, char *current_content)
{
    char *result = current_content;
    while (str[*i] && !ft_whitespace(str[*i]) && 
           !ft_is_operator(str[*i]) && 
           str[*i] != '\'' && str[*i] != '"')
        (*i)++;
    if (*i > *start)
	{
        result = append_content(current_content, &str[*start], *i - *start);
    }
    return result;
}

char *read_word(const char *str, int *i)
{
    int start = *i;
    char *content = NULL;

    while (str[*i] && !ft_whitespace(str[*i]) && !ft_is_operator(str[*i]))
    {
        if (str[*i] == '\'' || str[*i] == '"')
        {
            content = handle_unquoted_section(str, i, &start, content);
            if (!content) return NULL;
            content = handle_quoted_section(str, i, content);
            if (!content) return NULL;
            start = *i;
        }
        else
            (*i)++;
    }
    content = handle_unquoted_section(str, i, &start, content);
    if (!content)
        return NULL;
    return content;
}

token_type	get_operation_type(const char *op)
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
