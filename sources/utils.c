/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:05:56 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/24 13:58:18 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	protect_pwd_var(t_env **env_list)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->var_name, "PWD") == 0)
		{
			free(current->var_value);
			current->var_value = ft_strdup("/");
		}
		current = current->next;
	}
}


void	protect_cwd(t_env **env_list)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp && errno == ENOENT)
	{
		chdir("/");
		protect_pwd_var(env_list);
		printf("getcwd: cannot access parent directories: ");
		printf("No such file or directory\n");
	}
	free(tmp);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		free(cmd);
		cmd = tmp;
	}
}

void	save_fd(int flag)
{
	static int	out;
	static int	in;

	if (flag == 1)
	{
		out = dup(STDOUT_FILENO);
		in = dup(STDIN_FILENO);
	}
	if (flag == 2)
	{
		dup2(out, STDOUT_FILENO);
		dup2(in, STDIN_FILENO);
	}
}
