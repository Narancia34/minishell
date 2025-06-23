/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:51:22 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 21:09:44 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_all(t_shell *shell, char **args, t_here_docs *here_docs)
{
	free_commands(shell->input);
	clean_up(NULL, args);
	clean_up(NULL, shell->envp);
	free_env(&shell->env_list);
	if (here_docs)
		free_here_docs(here_docs);
}

void	handle_exec(char *path, char **args, t_shell *shell)
{
	if (execve(path, args, shell->envp) == -1)
	{
		perror("failed to execute command");
		clean_up(NULL, shell->envp);
		free_commands(shell->input);
		free_env(&shell->env_list);
		if (shell->pid_list)
			free_pids(shell->pid_list);
		clean_up(path, args);
		exit(126);
	}
}

void	exec_piped_cmd(t_shell *shell, char **args,
		char **o_args, t_here_docs *here_docs)
{
	char	*cmd_path;

	if (redirect(o_args, here_docs) == 1)
	{
		free_commands(shell->input);
		clean_up(NULL, args);
		clean_up(NULL, shell->envp);
		free_env(&shell->env_list);
		free_here_docs(here_docs);
		exit(EXIT_FAILURE);
	}
	free_here_docs(here_docs);
	cmd_path = find_cmd_path(args[0], shell->envp, &shell->exit_s);
	if (!cmd_path)
	{
		free_commands(shell->input);
		clean_up(NULL, args);
		clean_up(NULL, shell->envp);
		free_env(&shell->env_list);
		if (shell->pid_list)
			free_pids(shell->pid_list);
		exit(shell->exit_s);
	}
	handle_exec(cmd_path, args, shell);
}
