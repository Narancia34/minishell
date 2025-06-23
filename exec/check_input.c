/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:11:27 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/23 14:24:35 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_heredoc(t_check *check, t_shell *shell)
{
	check->here_docs = here_doc(shell);
	if (has_heredoc(shell->input) == 0 && !check->here_docs)
	{
		free_here_docs(check->here_docs);
		return (1);
	}
	return (0);
}

int	exec_input(t_check *check, t_shell *shell)
{
	if (is_builtin(check->args[0]) == 1)
	{
		check->ret = exec_builtin(shell, check->args,
				check->tmp->args, check->here_docs);
		if (check->ret != 257)
		{
			free_here_docs(check->here_docs);
			clean_up(NULL, check->args);
			return (check->ret);
		}
	}
	else
		exec_cmd(shell, check->args, check->tmp->args, check->here_docs);
	clean_up(NULL, check->args);
	return (257);
}

int	check_input(t_shell *shell)
{
	t_check	check;
	int		ret;

	if (check_heredoc(&check, shell) == 1)
		return (257);
	if (has_pipe(shell->input) > 1)
		handle_pipeline(shell, &check, check.here_docs);
	else
	{
		check.tmp = shell->input;
		while (check.tmp)
		{
			check.args = get_cmd(check.tmp->args);
			ret = exec_input(&check, shell);
			if (ret != 257)
				return (ret);
			check.tmp = check.tmp->next;
		}
	}
	if (check.here_docs)
		free_here_docs(check.here_docs);
	return (257);
}
