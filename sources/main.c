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
	t_env	*env_list;
	char	**u_env;
	t_var	*var_list;

	env_list = init_env(env);
	var_list = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break;
		if (*input)
			add_history(input);
		
		// Expand the input
		expanded_input = expand_input(input, 0, env_list);
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
		check_input(commands, env_list, u_env, tokens, var_list);
		/*clean_up(input, u_env);*/
		// while(commands)
		// {
		// 	if (commands->type == TOKEN_WORD)
		// 		printf("this token is a pipe\n");
		// 	commands = commands->next;
		// }
		//print_tokens(tokens);
		//print_commands(commands);

		// Free tokens and commands after use
		// Freeing logic should be added here
	}
	return 0;
}
// int main(int ac, char **av, char **env)
// {
// 	(void)av;
// 	(void)ac;
// 	char	*str;
// 	t_env	*list_env = NULL;
// 	char	*input;
// 	while(1)
// 	{
// 		init_env(env);
// 		str = readline("minishell: ");
// 		add_history(str);
// 		input = expand_input(str, 0, list_env);
// 		if (is_variable_assignment(str))
// 			handle_assignment(&list_env, input);
// 		else
// 			printf("%s\n", input);
// 	}
// }
