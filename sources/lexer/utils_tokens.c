/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:52:53 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 13:58:12 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static	char	*append_content(char *existing,
				const char *new_content, int len)
{
	char	*tmp;
	char	*result;

	if (!new_content || len <= 0)
		return (existing);
	tmp = ft_strndup(new_content, len);
	if (!tmp)
		return (existing);
	if (existing)
	{
		result = ft_strjoin(existing, tmp);
		free(existing);
		free(tmp);
		return (result);
	}
	else
		return (tmp);
}

static	char	*handle_quoted_section(const char *str,
				int *i, char *current_content)
{
	char	*quoted;
	char	*new_content;

	quoted = read_quoted((char *)str, i);
	if (!quoted)
	{
		free(current_content);
		return (NULL);
	}
	if (current_content)
	{
		new_content = ft_strjoin(current_content, quoted);
		free(current_content);
		free(quoted);
		return (new_content);
	}
	else
		return (quoted);
}

static	char	*handle_unquoted_section(const char *str,
				int *i, int *start, char *current_content)
{
	while (str[*i] && !ft_whitespace(str[*i]) && \
		!ft_is_operator(str[*i]) && \
		str[*i] != '\'' && str[*i] != '"')
	{
		(*i)++;
	}
	if (*i > *start)
		return (append_content(current_content, &str[*start], *i - *start));
	else
		return (current_content);
}

char	*read_word(const char *str, int *i, int *quote)
{
	int		start;
	char	*content;

	content = NULL;
	start = *i;
	while (str[*i] && !ft_whitespace(str[*i]) && !ft_is_operator(str[*i]))
	{
		if (str[*i] == '\'' || str[*i] == '"')
		{
			quote[('\'' - str[*i]) != 0] = 1;
			content = handle_unquoted_section(str, i, &start, content);
			content = handle_quoted_section(str, i, content);
			if (!content)
				return (NULL);
			start = *i;
		}
		else
			(*i)++;
	}
	content = handle_unquoted_section(str, i, &start, content);
	if (!content)
		return (ft_strdup(""));
	return (content);
}
