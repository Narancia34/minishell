/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_paths_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:42:46 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 20:46:34 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_up(char *str, char **strs)
{
	int	i;

	if (str)
		free(str);
	if (strs)
	{
		i = 0;
		while (strs[i])
			free(strs[i++]);
		free(strs);
	}
}

char	*find_pre_path(char **envp)
{
	int		i;
	char	*res;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	res = ft_strdup(envp[i] + 5);
	return (res);
}

char	*get_command(char *full_cmd)
{
	char	**cmds;
	char	*res;

	cmds = ft_split(full_cmd, ' ');
	if (!cmds)
		return (NULL);
	res = ft_strdup(cmds[0]);
	return (res);
}

char	*cat_path_cmd(char *pre_path, char *full_cmd)
{
	char	*res;
	char	*tmp;
	char	*cmd;

	cmd = ft_strdup(full_cmd);
	if (!cmd)
		return (NULL);
	tmp = ft_strjoin(pre_path, "/");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	free(cmd);
	return (res);
}
