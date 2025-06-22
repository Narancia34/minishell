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

int	is_digits(char	*arg)
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
		return(2);
	}
	if (arg[2])
	{
		printf("exit\nminishell: exit: too many arguments\n");
		return 257;
	}
	printf("exit\n");
	return (ft_atoi(arg[1])  % 256);
}

t_env *new_node_from_kv(const char *name, const char *value)
{
	t_env *node;

	node = malloc(sizeof(t_env));
	node->var_name = ft_strdup(name);
	node->var_value = ft_strdup(value);
	node->flag = 0;
	node->exported = 1;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	set_pwd_env(t_env **env_list, char *old_dir, char *current_dir, t_pwd *pwd)
{
	t_env	*new_oldpwd;
	t_env	*new_pwd;

	if (!pwd->oldpwd)
	{
		new_oldpwd = new_node_from_kv("OLDPWD", old_dir);
		add_node(env_list, new_oldpwd);
	}
	if (!pwd->pwd)
	{
		new_pwd = new_node_from_kv("PWD", current_dir);
		add_node(env_list, new_pwd);
	}
}

void	change_pwd_env(char *old_dir, char *current_dir, t_env **env_list)
{
	t_env *current;
	t_pwd *pwd;

	pwd = malloc(sizeof(t_pwd));
	pwd->pwd = 0;
	pwd->oldpwd = 0;
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, "OLDPWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(old_dir);
			pwd->oldpwd = 1;
		}
		else if (ft_strcmp(current->var_name, "PWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup(current_dir);
			pwd->pwd = 1;
		}
		current = current->next;
	}
	set_pwd_env(env_list, old_dir, current_dir, pwd);
	free(pwd);
}

int	cd_home(t_env *env_list)
{
	t_env	*tmp;
	char	*path;

	path = NULL;
	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp("HOME", tmp->var_name) == 0)
		{
			path = ft_strdup(tmp->var_value);
			break;
		}
		tmp = tmp->next;
	}
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(path) != 0)
	{
		perror("minishell: cd: error\n");
		return (free(path), 1);
	}
	return (free(path), 0);
}

int	cd_path(char *path)
{
	if (chdir(path) != 0)
	{
		perror("minishell: cd: error\n");
		return (1);
	}
	return (0);
}

int	ft_cd(char **arg, t_env **env_list)
{
	char	current_dir[1024];
	char	old_dir[1024];
	char	*path;

	path = NULL;
	getcwd(old_dir, sizeof(old_dir));
	if (arg[1] == NULL)
	{
		if (cd_home(*env_list) == 1)
			return (1);
	}
	else
	{
		if (cd_path(arg[1]) == 1)
			return (1);
	}
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("minishell: cd: getcwd error");
		return 1;
	}
	change_pwd_env(old_dir, current_dir, env_list);
	return (0);
}

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
			break;
		print_newline = 0;
		i++;
	}
	print_arg(arg, i);
	if (print_newline)
		printf("\n");
	return (0);
}

int	ft_pwd()
{
	char	current_dir[1024];

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
		return (1);
	printf("%s\n", current_dir);
	return (0);
}

int	ft_env(t_env *env_list)
{
	int		i;
	char	**res;

	res = upd_env(env_list);
	if (!res || !res[0])
		return (1);
	i = 0;
	while (res[i])
	{
		printf("%s\n", res[i]);
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}

void remove_env_var(t_env **env_list, char *var_name)
{
	t_env *current;

	current = *env_list;
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

int	ft_unset(char **args, t_env **env_list)
{
	int	i;

	if (!args || !args[0] || !env_list || !*env_list)
		return (1);
	i = 1;
	while (args[i])
	{
		remove_env_var(env_list, args[i]);
		i++;
	}
	return (0);
}

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

typedef struct s_export
{
	char	*var_name;
	char	*var_value;
	t_env	*new;
	int		i;
	int		j;
	int		flag;
	t_env	*tmp;
}	t_export;

int	init_export(t_export *export, char *arg)
{
	export->i = 0;
	export->flag = 1;
	while (arg[export->i] != '=' && arg[export->i])
		export->i++;
	export->var_name = malloc(sizeof(char) * (export->i + 1));
	if (arg[export->i] == '=')
		export->var_value = ft_strdup(arg + export->i + 1);
	else
		export->var_value = ft_strdup("");
	export->j = 0;
	while (arg[export->j] != '=' && arg[export->j])
	{
		export->var_name[export->j] = arg[export->j];
		export->j++;
	}
	if (arg[export->j] == '=')
		export->flag = 0;
	export->var_name[export->j] = '\0';
	if (!ft_isalpha(export->var_name[0]) && export->var_name[0] != '_')
	{
		free(export->var_name);
		free(export->var_value);
		return (1);
	}
	return (0);
}

int	export_vars(t_export *export, t_env **env_list)
{
	export->tmp = *env_list;
	while (export->tmp)
	{
		if (ft_strcmp(export->var_name, export->tmp->var_name) == 0)
		{
			if (export->flag == 1)
			{
				free(export->var_name);
				free(export->var_value);
				return (0);
			}
			export->tmp->flag = 0;
			free(export->tmp->var_value);
			export->tmp->var_value = ft_strdup(export->var_value);
			free(export->var_name);
			free(export->var_value);
			return (0);
		}
		export->tmp = export->tmp->next;
	}
	return (1);
}

int	add_to_env(t_env **env_list, char *arg)
{
	t_export	export;

	if (init_export(&export, arg) == 1)
		return (1);
	export.i = 1;
	if (export_vars(&export, env_list) == 0)
		return (0);
	while (export.var_name[export.i])
	{
		if (!ft_isalnum(export.var_name[export.i])
			&& export.var_name[export.i] != '_')
		{
			free(export.var_name);
			free(export.var_value);
			return (1);
		}
		export.i++;
	}
	export.new = make_node(export.var_name, export.var_value, export.flag);
	add_to_list(env_list, export.new);
	free(export.var_name);
	free(export.var_value);
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
