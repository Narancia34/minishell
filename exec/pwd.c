/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:18:25 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:18:51 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(void)
{
	char	current_dir[1024];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
		return (1);
	printf("%s\n", current_dir);
	return (0);
}
