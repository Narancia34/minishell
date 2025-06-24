/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:09:50 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 13:59:44 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_flag = 0;
// void print_commands(t_command *cmd)
// {
//     int i = 0;
//     while (cmd)
//     {
//         printf("Command:\n");
//         for (i = 0; i < cmd->arg_size; i++)
//             printf("  Arg[%d]: %s\n", i, cmd->args[i]);
//         printf("  Type: %d\n", cmd->type);
//         cmd = cmd->next;
//     }
// }
//
// void print_tokens(t_token *list)
// {
//     while (list)
//     {
//         printf("Token: %-10s | Type: %d\n", list->value, list->type);
//         list = list->next;
//     }
// }

void	innit_shell(t_shell *shell, char **env)
{
	shell->exit_s = 0;
	shell->env_list = init_env(env);
	handle_shlvl(&shell->env_list);
	setup_signals();
}

int	ft_read_line(t_shell *shell)
{
	g_signal_flag = 0;
	shell->rl = readline("minishell$ ");
	if (g_signal_flag == SIGINT)
		shell->exit_s = 130;
	if (!shell->rl)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (1);
	}
	if (*shell->rl)
		add_history(shell->rl);
	return (0);
}

int	parse_utils(t_shell *shell)
{
	shell->tokens = tokenize(shell->rl);
	if (!shell->tokens)
	{
		free(shell->rl);
		return (1);
	}
	free(shell->rl);
	expand_tokens(shell->tokens, shell->exit_s, shell->env_list);
	if (!validate_syntax(shell->tokens))
	{
		shell->exit_s = 2;
		free_tokens(shell->tokens);
		return (1);
	}
	shell->input = build_commands(shell->tokens);
	if (!shell->input)
	{
		free_tokens(shell->tokens);
		return (1);
	}
	free_tokens(shell->tokens);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int		ret;
	t_shell	shell;

	(void)av;
	(void)ac;
	innit_shell(&shell, env);
	while (1)
	{
		if (ft_read_line(&shell) == 1)
			break ;
		if (parse_utils(&shell) == 1)
			continue ;
		shell.envp = upd_env(shell.env_list);
		ret = check_input(&shell);
		if (ret != 257)
		{
			free_commands(shell.input);
			clean_up(NULL, shell.envp);
			free_env(&(shell.env_list));
			exit(ret);
		}
		free_commands(shell.input);
		clean_up(NULL, shell.envp);
	}
	return (free_env(&shell.env_list), 0);
}
