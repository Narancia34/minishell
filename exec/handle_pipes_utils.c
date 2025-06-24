/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:00:17 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/24 10:04:06 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_for_child(t_shell *shell, t_pipeline *pipeline)
{
	clean_up(NULL, pipeline->args);
	clean_up(NULL, shell->envp);
	free_commands(shell->input);
	free_env(&shell->env_list);
	free_pids(shell->pid_list);
	if (pipeline->here_docs_head)
		free_here_docs(pipeline->here_docs_head);
	exit (shell->exit_s);
}

void	free_for_child_b(t_shell *shell, t_pipeline *pipeline)
{
	clean_up(NULL, pipeline->args);
	clean_up(NULL, shell->envp);
	free_commands(shell->input);
	free_env(&shell->env_list);
	free_pids(shell->pid_list);
	free_here_docs(pipeline->saved_head);
	exit(shell->exit_s);
}

void	handle_child_p(t_shell *shell,
		t_here_docs *here_docs, t_pipeline *pipeline)
{
	pipeline->here_docs_head = here_docs;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_pipe_util_a(pipeline->prev_fd, pipeline->fd, pipeline->tmp);
	pipeline->args = get_cmd(pipeline->tmp->args);
	if (!pipeline->args)
	{
		perror("minishell: error parsing command!/n");
		pipeline->tmp = pipeline->tmp->next;
		free_for_child(shell, pipeline);
	}
	if (is_builtin(pipeline->args[0]) == 1)
		exec_builtin(shell, pipeline->args,
			pipeline->tmp->args, pipeline->here_docs_head);
	else
		exec_piped_cmd(shell, pipeline->args,
			pipeline->tmp->args, pipeline->here_docs_head);
	free_for_child_b(shell, pipeline);
}

void	handle_pipe_util_c(t_shell *shell,
		t_pipeline *pipeline, t_here_docs **here_docs)
{
	pipeline->tmp_n = make_pid_node(pipeline->pid);
	add_pid_node(&shell->pid_list, pipeline->tmp_n);
	pipeline->count = count_here_docs(pipeline->tmp->args);
	while (pipeline->count > 0)
	{
		*here_docs = (*here_docs)->next;
		pipeline->count--;
	}
	pipeline->tmp = pipeline->tmp->next;
}

void	wait_for_pipes(t_shell *shell, t_pipeline *pipeline)
{
	pipeline->tmp_n = shell->pid_list;
	while (pipeline->tmp_n)
	{
		waitpid(pipeline->tmp_n->pid, &pipeline->status, 0);
		pipeline->to_free = pipeline->tmp_n;
		pipeline->tmp_n = pipeline->tmp_n->next;
		free(pipeline->to_free);
	}
}
