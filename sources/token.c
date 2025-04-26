/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:39:54 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/08 16:49:32 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char *read_quoted(char *input, int *i)
{
    char quote = input[*i];
    int start = ++(*i);

    while (input[*i] && input[*i] != quote)
        (*i)++;
    if (input[*i] != quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n",STDERR_FILENO);
        return (NULL);
    }
    int len = *i - start;
    char *res = ft_strndup(&input[start], len);
    (*i)++;
    return res;
}

char *read_operator(const char *str, int *i)
{
    int start = *i;
    if ((str[*i] == '>' || str[*i] == '<') && str[*i] == str[*i + 1])
        *i += 2;
    else
        (*i)++;
    return ft_strndup(&str[start], *i - start);
}

char *read_word(const char *str, int *i)
{
    int start = *i;
    while (str[*i] && !ft_whitespace(str[*i]) && !ft_is_operator(str[*i]) && str[*i] != '\'' && str[*i] != '"')
        (*i)++;
    return ft_strndup(&str[start], *i - start);
}

token_type	get_operation_type(const char *op)
{
	if (ft_strcmp(op, ">") == 0)
		return (TOKEN_REDIR_OUT);
	if (ft_strcmp(op , ">>") == 0)
		return (TOKEN_REDIR_APPEND);
	if (ft_strcmp(op, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(op, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(op, "|") == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

// void	static	ft_help(t_token	*token_list, const char *input)
// {
// 	int		i;
// 	char	*quoted;
//
// 	i = 0;
// 	if (input[i] == '\'' || input[i] == '"')
// 	{
// 		quoted = read_quoted((char *)input, &i);
// 		ft_add_token(&token_list, quoted, TOKEN_WORD);
// 		free(quoted);
// 	}
// }

t_token	*tokenize(const char *input)
{
	t_token	*token_list = NULL;
	int	i = 0;
	while(input[i])
	{
		while(input[i] && ft_whitespace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '"')
		{
			char *quoted = read_quoted((char *)input, &i);
			if (!quoted)
				return 0;
			ft_add_token(&token_list, quoted, TOKEN_WORD);
			free(quoted);
		}
		else if (ft_is_operator(input[i]))
		{
			char *op = read_operator(input, &i);
			token_type type = get_operation_type(op);
			ft_add_token(&token_list, op, type);
		}
		else if (input[i] != '\0')
		{
			char *word = read_word(input, &i);
			ft_add_token(&token_list, word, TOKEN_WORD);
			free(word);
		}
	}
	return token_list;
}
