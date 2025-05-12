/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:47:37 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/11 09:49:17 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	change_pwd_env(char *old_dir, char *current_dir, t_env **env_list)
{
	t_env *current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, "OLDPWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(old_dir);
		}
		else if (ft_strcmp(current->var_name, "PWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(current_dir);
		}
		current = current->next;
	}
}

int	ft_cd(char **arg, t_env **env_list)
{
	char	current_dir[1024];
	char	old_dir[1024];
	char	*path;

	getcwd(old_dir, sizeof(old_dir));
	if (arg[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return 1;
		}
	}
	else
		path = arg[1];
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		return 1;
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("minishell: cd: getcwd error");
		return 1;
	}
	change_pwd_env(old_dir, current_dir, env_list);
	return 0;
}

void	ft_echo(char **arg)
{
	int	i = 1;
	int	print_newline = 1;
	while (arg[i] && arg[i][0] == '-' && arg[i][1] == 'n')
	{
		int	j = 2;
		while (arg[i][j] == 'n')
			j++;
		if (arg[i][j] != '\0')
			break;
		print_newline = 0;
		i++;
	}
	int	first = 1;
	while (arg[i])
	{
		if (!first)
			printf(" ");
		printf("%s", arg[i]);
		first = 0;
		i++;
	}
	if (print_newline)
		printf("\n");
}

void	ft_pwd()
{
	char	current_dir[1024];

	getcwd(current_dir, sizeof(current_dir));
	printf("%s\n", current_dir);
}

void	ft_env(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		printf("%s=%s\n", tmp->var_name, tmp->var_value);
		tmp = tmp->next;
	}
}

void remove_env_var(t_env **env_list, char *var_name)
{
	t_env *current = *env_list;

	while (current)
	{
		if (ft_strcmp(current->var_name, var_name) == 0)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				*env_list = current->next;
			if (current->next)
				current->next->prev = current->prev;
			free(current->var_name);
			free(current->var_value);
			free(current);
			return;
		}
		current = current->next;
	}
}

void	ft_unset(char **args, t_env **env_list)
{
	int	i;

	if (!args || !args[0] || !env_list || !*env_list)
		return;

	i = 1;
	while (args[i])
	{
		remove_env_var(env_list, args[i]);
		i++;
	}
}

void	print_export(t_env	*env_list)
{
	(void)env_list;
	return ;
}

void	add_to_env(t_env **env_list, char *arg)
{
	char	*var_name;
	char	*var_value;
	t_env	*new;
	int		i;
	int		j;

	i = 0;
	while (arg[i] != '=' && arg[i])
		i++;
	var_name = malloc(sizeof(char) * (i + 1));
	var_value = ft_strdup(arg + i + 1);
	j = 0;
	while (arg[j] != '=' && arg[j])
	{
		var_name[j] = arg[j];
		j++;
	}
	var_name[j] = '\0';
	new = make_node(var_name, var_value);
	add_to_list(env_list, new);
}

void	ft_export(char **args, t_env **env_list)
{
	int		i;

	i = 1;
	if (!args[i])
		print_export(*env_list);
	else
	{
		while (args[i])
		{
			add_to_env(env_list, args[i]);
			i++;
		}
	}

}
