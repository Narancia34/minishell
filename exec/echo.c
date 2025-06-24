/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:13:43 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:14:06 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_arg(char **arg, int i)
{
	int	first;
	int	j;

	j = i;
	first = 1;
	while (arg[j])
	{
		if (!first)
			printf(" ");
		printf("%s", arg[j]);
		first = 0;
		j++;
	}
}

int	ft_echo(char **arg)
{
	int	i;
	int	j;
	int	print_newline;

	i = 1;
	print_newline = 1;
	while (arg[i] && arg[i][0] == '-' && arg[i][1] == 'n')
	{
		j = 2;
		while (arg[i][j] == 'n')
			j++;
		if (arg[i][j] != '\0')
			break ;
		print_newline = 0;
		i++;
	}
	print_arg(arg, i);
	if (print_newline)
		printf("\n");
	return (0);
}
