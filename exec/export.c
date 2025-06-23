/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:23:34 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 11:24:45 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	switch_value(char **tmp, char **arg, char **min_arg)
{
	*tmp = *arg;
	*arg = *min_arg;
	*min_arg = *tmp;
}

char	**sort_env(t_env *env_list, int count)
{
	char	**res;
	char	*tmp;
	int		i;
	int		j;
	int		min_index;

	res = upd_env(env_list);
	if (!res)
		return (NULL);
	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		min_index = i;
		while (j < count)
		{
			if (ft_strcmp(res[j], res[min_index]) < 0)
				min_index = j;
			j++;
		}
		if (min_index != i)
			switch_value(&tmp, &res[i], &res[min_index]);
		i++;
	}
	return (res);
}

int	print_export(t_env *env_list)
{
	char	**res;
	int		i;
	int		count;

	count = lstlen(env_list);
	res = sort_env(env_list, count);
	i = 0;
	while (i < count)
	{
		printf("%s\n", res[i]);
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}

int	ft_export(char **args, t_env **env_list)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	if (!args[i])
		ret = print_export(*env_list);
	else
	{
		while (args[i])
		{
			ret = add_to_env(env_list, args[i]);
			i++;
		}
	}
	return (ret);
}
