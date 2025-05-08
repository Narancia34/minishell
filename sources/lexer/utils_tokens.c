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

char	*read_quoted(char *input, int *i)
{
	char	*result = malloc(strlen(input) + 1);
	int		j = 0;
	char	quote;

	if (!result)
		return NULL;
	while (input[*i])
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[(*i)++];
			while (input[*i] && input[*i] != quote)
				result[j++] = input[(*i)++];
			if (input[*i] == quote)
				(*i)++;
			else
			{
				ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
				free(result);
				return NULL;
			}
		}
		else if (input[*i] == ' ' || ft_is_operator(input[*i]))
			break;
		else
			result[j++] = input[(*i)++];
	}
	result[j] = '\0';
	return result;
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

char	*read_word(const char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && !ft_whitespace(str[*i]) && \
		!ft_is_operator(str[*i]) && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	return (ft_strndup(&str[start], *i - start));
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
