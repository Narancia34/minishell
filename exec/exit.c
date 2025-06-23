/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:06:44 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:07:09 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_digits(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			printf("exit\nexit: %s: numeric argument required\n", arg);
			return (2);
		}
		i++;
	}
	return (ft_atoi(arg));
}

int	ft_exit(char **arg)
{
	int	status;

	if (!arg[1])
	{
		printf("exit\n");
		return (0);
	}
	status = is_digits(arg[1]);
	if (status == 2)
	{
		return (2);
	}
	if (arg[2])
	{
		printf("exit\nminishell: exit: too many arguments\n");
		return (257);
	}
	printf("exit\n");
	return (ft_atoi(arg[1]) % 256);
}
