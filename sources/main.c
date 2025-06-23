/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:09:50 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/23 12:25:07 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
volatile sig_atomic_t g_signal_flag = 0;

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

int main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	int	ret;
	t_shell	shell;
	
	shell.exit_s = 0;
	shell.env_list = init_env(env);
	handle_shlvl((&shell.env_list));
	setup_signals();
	while (1)
	{
		g_signal_flag = 0;
		shell.rl = readline("minishell$ ");
		if (g_signal_flag == SIGINT)
			shell.exit_s = 130;
		if (!shell.rl)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (*shell.rl)
			add_history(shell.rl);
		shell.tokens = tokenize(shell.rl);
		if (!shell.tokens)
		{
			free(shell.rl);
			continue;
		}
		free(shell.rl);
		expand_tokens(shell.tokens, shell.exit_s, shell.env_list);
		if (!validate_syntax(shell.tokens))
		{
			shell.exit_s = 2;
			free_tokens(shell.tokens);
			continue;
		}
		shell.input = build_commands(shell.tokens);
		if (!shell.input) {
			free_tokens(shell.tokens);
			continue;
		}
		free_tokens(shell.tokens);
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
	free_env(&shell.env_list);
	return 0;
}
