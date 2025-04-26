/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:28:35 by mgamraou          #+#    #+#             */
/*   Updated: 2025/04/11 11:12:35 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_env	*env_list;
	char	*input;
	char	**u_env;

	env_list = init_env(env);
	if (!env_list)
	{
		perror("Environment initialization failed");
		return (1);
	}
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break;
		if (*input)
			add_history(input);
		t_token *tokens = tokenize(input);
		expand_input(input, 0, env_list);
		if (!validate_syntax(tokens))
		{
			free(input);
			continue;
		}
		t_command *commands = build_commands(tokens);
		u_env = upd_env(env_list);
		check_input(commands, env_list, u_env);
		clean_up(input, u_env);
	}
}
