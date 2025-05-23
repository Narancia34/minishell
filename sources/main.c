/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:09:50 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/05/21 10:50:48 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

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

int main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	char *input;
	char *expanded_input;
	t_env    *env_list;
	char	**u_env;
	t_var    *var_list;

	env_list = init_env(env);
	var_list = NULL;
	handle_shlvl(&env_list);
	setup_signals();
	while (1)
	{
		g_signal_flag = 0;
		input = readline("minishell$ ");
		if (g_signal_flag == SIGINT)
		{
		}
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (*input)
			add_history(input);

		// Expand the input
		expanded_input = expand_input(input, 0, env_list, var_list);
		free(input); // Free the original input after expansion
		if (!expanded_input)
			continue; // Skip if expansion fails
		// Tokenize the expanded input
		t_token *tokens = tokenize(expanded_input);
		free(expanded_input); // Free expanded input after tokenizing
		if (!validate_syntax(tokens))
		{
			return 0;
		}
		t_command *commands = build_commands(tokens);
		u_env = upd_env(env_list);
		check_input(commands, &env_list, u_env, tokens, &var_list);
	}
	return 0;
}
