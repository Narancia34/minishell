/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hande_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:41:34 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 20:45:45 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*check_access(char **pre_paths, char *full_cmd, int *exit_s)
{
	int			i;
	char		*res;
	struct stat	sb;

	i = 0;
	while (pre_paths[i])
	{
		res = cat_path_cmd(pre_paths[i], full_cmd);
		if (res && stat(res, &sb) == 0)
		{
			if ((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP)
				|| (sb.st_mode & S_IXOTH))
			{
				clean_up(NULL, pre_paths);
				*exit_s = 0;
				return (res);
			}
		}
		free(res);
		i++;
	}
	printf("command not found\n");
	*exit_s = 127;
	clean_up(NULL, pre_paths);
	return (NULL);
}

char	*check_file(char *full_cmd, t_path *path, int	*exit_s)
{
	if (S_ISDIR(path->sb.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", full_cmd);
		*exit_s = 126;
		return (NULL);
	}
	if ((path->sb.st_mode & S_IXUSR) || (path->sb.st_mode & S_IXGRP)
		|| (path->sb.st_mode & S_IXOTH))
		return (ft_strdup(full_cmd));
	else
	{
		printf("minishell: %s: Permission denied\n", full_cmd);
		*exit_s = 126;
		return (NULL);
	}
}

void	*no_such_file(int *exit_s, char *full_cmd)
{
	printf("minishell: %s: no such file or directory\n", full_cmd);
	*exit_s = 127;
	return (NULL);
}

char	*get_path(char *full_cmd, int *exit_s, t_path *path)
{
	if (!path->pre_path)
	{
		*exit_s = 127;
		printf("command not found\n");
		return (NULL);
	}
	path->pre_paths = ft_split(path->pre_path, ':');
	free(path->pre_path);
	if (!path->pre_paths)
		return (NULL);
	path->res = check_access(path->pre_paths, full_cmd, exit_s);
	return (path->res);
}

char	*find_cmd_path(char *full_cmd, char **envp, int *exit_s)
{
	t_path	path;

	if (!full_cmd || !*full_cmd)
		return (NULL);
	if (full_cmd[0] == '.' || full_cmd[0] == '/')
	{
		if (stat(full_cmd, &path.sb) == 0)
			return (check_file(full_cmd, &path, exit_s));
		else
			return (no_such_file(exit_s, full_cmd));
	}
	if (stat(full_cmd, &path.sb) == 0)
		return (check_file(full_cmd, &path, exit_s));
	if (!envp || !envp[0])
	{
		*exit_s = 127;
		return (NULL);
	}
	path.pre_path = find_pre_path(envp);
	return (get_path(full_cmd, exit_s, &path));
}
