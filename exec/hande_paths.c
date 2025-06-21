/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hande_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:41:34 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/18 13:41:10 by mgamraou         ###   ########.fr       */
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

	cmds =  ft_split(full_cmd, ' ');
	if (!cmds)
		return (NULL);
	res = ft_strdup(cmds[0]);
	//clean_up(NULL, cmds);
	return (res);
}

char	*cat_path_cmd(char *pre_path, char *full_cmd)
{
	char	*res;
	char	*tmp;
	char	*cmd;

	//cmd = get_command(full_cmd);
	cmd = ft_strdup(full_cmd);
	if (!cmd)
		return (NULL);
	tmp = ft_strjoin(pre_path, "/");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	free(cmd);
	return (res);
}

/*char	*check_access(char **pre_paths, char *full_cmd)*/
/*{*/
/*	int		i;*/
/*	char	*res;*/
/**/
/*	i = 0;*/
/*	while (pre_paths[i])*/
/*	{*/
/*		res = cat_path_cmd(pre_paths[i], full_cmd);*/
/*		if (res && access(res, F_OK) == 0)*/
/*			return (clean_up(NULL, pre_paths), res);*/
/*		free(res);*/
/*		i++;*/
/*	}*/
/*	return (NULL);*/
/*}*/
/**/
/*char	*find_cmd_path(char *full_cmd, char **envp)*/
/*{*/
/*	char	*res;*/
/*	char	*pre_path;*/
/*	char	**pre_paths;*/
/**/
/*	if (!full_cmd || !*full_cmd)*/
/*		return (NULL);*/
/*	if (full_cmd[0] == '.' || full_cmd[0] == '/')*/
/*	{*/
/*		if (access(full_cmd, F_OK) == 0)*/
/*			return (ft_strdup(full_cmd));*/
/*		else*/
/*			return (NULL);*/
/*	}*/
/*	if (!envp || !envp[0])*/
/*		return (NULL);*/
/*	pre_path = find_pre_path(envp);*/
/*	if (!pre_path)*/
/*		return (NULL);*/
/*	pre_paths = ft_split(pre_path, ':');*/
/*	free(pre_path);*/
/*	if (!pre_paths)*/
/*		return (NULL);*/
/*	res = check_access(pre_paths, full_cmd);*/
/*	if (!res || !*res)*/
/*		return (clean_up(NULL, pre_paths), NULL);*/
/*	return (res);*/
/*}*/


char	*check_access(char **pre_paths, char *full_cmd, int *exit_s)
{
	int		i;
	char	*res;
	struct stat	sb;

	i = 0;
	while (pre_paths[i])
	{
		res = cat_path_cmd(pre_paths[i], full_cmd);
		if (res && stat(res, &sb) == 0)
		{
			if ((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH))
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

char	*find_cmd_path(char *full_cmd, char **envp, int *exit_s)
{
	char	*res;
	char	*pre_path;
	char	**pre_paths;
	struct stat sb;

	if (!full_cmd || !*full_cmd)
		return (NULL);
	if (full_cmd[0] == '.' || full_cmd[0] == '/')
	{
		if (stat(full_cmd, &sb) == 0)
		{
			if (S_ISDIR(sb.st_mode))
			{
				fprintf(stderr, "minishell: %s: Is a directory\n", full_cmd);
				*exit_s = 126;
				return (NULL);
			}
			if ((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH))
				return (ft_strdup(full_cmd));
			else
			{
				fprintf(stderr, "minishell: %s: Permission denied\n", full_cmd);
				*exit_s = 126;
				return (NULL);
			}
		}
		else
	{
			ft_putstr_fd("no such file or directory\n", 2);
			*exit_s = 127;
			return (NULL);
		}
	}
	if (stat(full_cmd, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			fprintf(stderr, "minishell: %s: Is a directoy\n", full_cmd);
			*exit_s = 126;
			return (NULL);
		}
		if ((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH))
			return (ft_strdup(full_cmd));
		else
		{
			fprintf(stderr, "minishell: %s: Permission denied\n", full_cmd);
			*exit_s = 126;
			return (NULL);
		}
	}
	if (!envp || !envp[0])
	{
		*exit_s = 127;
		return (NULL);
	}
	pre_path = find_pre_path(envp);
	if (!pre_path)
	{
		*exit_s = 127;
		printf("command not found\n");
		return (NULL);
	}
	pre_paths = ft_split(pre_path, ':');
	free(pre_path);
	if (!pre_paths)
		return (NULL);
	res = check_access(pre_paths, full_cmd, exit_s);
	return (res);
}
