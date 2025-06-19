/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:09:50 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/12 12:36:07 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	free_tokens(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

void free_commands(t_command *cmd) {
    t_command *tmp;
    int i;
    while (cmd) {
        tmp = cmd->next;
        if (cmd->args) {
            for (i = 0; cmd->args[i]; i++)
                free(cmd->args[i]);
            free(cmd->args);
        }
        free(cmd);
        cmd = tmp;
    }
}

void print_commands(t_command *cmd)
{
    int i = 0;
    while (cmd)
    {
        printf("Command:\n");
        for (i = 0; i < cmd->arg_size; i++)
            printf("  Arg[%d]: %s\n", i, cmd->args[i]);
        printf("  Type: %d\n", cmd->type);
        cmd = cmd->next;
    }
}

void print_tokens(t_token *list)
{
    while (list)
    {
        printf("Token: %-10s | Type: %d\n", list->value, list->type);
        list = list->next;
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


int main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	char *input;
	t_env    *env_list;
	char	**u_env;
	t_var    *var_list;
	int		exit_s;
	int	ret;
	t_token *tokens;
	t_command *commands;

	exit_s = 0;
	env_list = init_env(env);
	var_list = NULL;
	handle_shlvl(&env_list);
	setup_signals();
	while (1)
	{
		g_signal_flag = 0;
		protect_cwd(&env_list);
		input = readline("minishell$ ");
		if (g_signal_flag == SIGINT)
		{
			exit_s = 130;
		}
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		if (!tokens)
		{
			continue;
			free(input);
		}
		expand_tokens(tokens, exit_s, env_list);
		if (!validate_syntax(tokens))
		{
			exit_s = 2;
			free_tokens(tokens);
			continue;
		}
		commands = build_commands(tokens);
		if (!commands) {
			free_tokens(tokens);
			continue;
		}
		u_env = upd_env(env_list);
		free_tokens(tokens);
		ret = check_input(commands, &env_list, u_env, &exit_s);
		if (ret != 257)
		{
			free_commands(commands);
			clean_up(NULL, u_env);
			free_env(&env_list);
			exit(ret);
		}
		free_commands(commands);
		clean_up(NULL, u_env);
	}
	t_env	*env_to_free;
	while (env_list)
	{
		free(env_list->var_value);
		free(env_list->var_name);
		env_to_free = env_list;
		env_list = env_list->next;
		free(env_to_free);
	}
	return 0;
}
