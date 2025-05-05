/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:07:13 by mgamraou          #+#    #+#             */
/*   Updated: 2025/05/03 18:27:51 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_var	*make_node(char *var_name, char *var_value)
{
	t_var	*node;

	node = (t_var *)malloc(sizeof(t_var));
	if (!node)
		return (NULL);
	node->var_name = ft_strdup(var_name);
	if (!node->var_name)
		return (free(node), NULL);
	if (var_value)
	{
		node->var_value = ft_strdup(var_value);
		if (!node->var_value)
		{
			free(node->var_name);
			return (free(node), NULL);
		}
	}
	else
		node->var_value = NULL;
	node->next = NULL;
	return (node);
}

void	add_to_list(t_var **var_list, t_var *new)
{
	t_var	*current;

	if (!*var_list)
	{
		*var_list = new;
		return;
	}
	current = *var_list;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	handle_var(t_var **var_list, char *arg)
{
	char	*var_name;
	char	*var_value;
	t_var	*new;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(arg);
	i = 0;
	while (arg[i] != '=')
		i++;
	var_name = malloc(sizeof(char) * i + 1);
	var_value = ft_strdup(arg + i + 1);
	j = 0;
	while (arg[j] != '=')
	{
		var_name[j] = arg[j];
		j++;
	}
	var_name[j] = '\0';
	new = make_node(var_name, var_value);
	add_to_list(var_list, new);
}
