/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:26:23 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/24 12:18:44 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	here_doc_utils(t_here_d *here_d, char *delimiter)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	here_d->res = ft_strdup("");
	while (1)
	{
		here_d->line = readline(">");
		if (!here_d->line)
		{
			ft_putstr_fd("here-document delimited by end-of-file\n", 2);
			break ;
		}
		if (ft_strcmp(here_d->line, delimiter) == 0)
			break ;
		here_d->tmp = here_d->line;
		here_d->line = ft_strjoin(here_d->line, "\n");
		free(here_d->tmp);
		here_d->tmp = here_d->res;
		here_d->res = ft_strjoin(here_d->res, here_d->line);
		free (here_d->tmp);
		free (here_d->line);
	}
}

void	here_doc_utils_b(t_here_d *here_d,
			t_command *tmp_c, t_shell *shell, t_here_docs *here_docs)
{
	here_d->expanded = expand_env_vars(here_d->res,
			shell->exit_s, shell->env_list, tmp_c->flag);
	ft_putstr_fd(here_d->expanded, here_d->fd);
	free(here_d->expanded);
	free(here_d->res);
	close(here_d->fd);
	free_env(&shell->env_list);
	free_commands(shell->input);
	clean_up(here_d->file_name, shell->envp);
	free_here_docs(here_docs);
	exit(EXIT_SUCCESS);
}

char	*here_doc_utils_c(t_here_d *here_d, t_shell *shell)
{
	waitpid(here_d->pid, &here_d->status, 0);
	if (WIFEXITED(here_d->status))
		shell->exit_s = WEXITSTATUS(here_d->status);
	else if (WIFSIGNALED(here_d->status))
	{
		shell->exit_s = 130;
		printf("\n");
		unlink(here_d->file_name);
		free(here_d->file_name);
		setup_signals();
		g_signal_flag = WTERMSIG(here_d->status);
		return (NULL);
	}
	setup_signals();
	return (here_d->file_name);
}

char	*handle_here_doc(t_shell *shell, t_command *tmp_c, t_hd *hd)
{
	t_here_d	here_d;

	ignore_signals();
	here_d.file_name = make_file_name();
	here_d.pid = fork();
	if (here_d.pid == 0)
	{
		here_doc_utils(&here_d, hd->tmp->args[hd->i + 1]);
		here_d.fd = open(here_d.file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (here_d.fd == -1)
		{
			perror("open()");
			return (NULL);
		}
		here_doc_utils_b(&here_d, tmp_c, shell, hd->here_docs);
	}
	return (here_doc_utils_c(&here_d, shell));
}

t_here_docs	*here_doc(t_shell *shell)
{
	t_hd	hd;

	hd.here_docs = NULL;
	hd.tmp = shell->input;
	while (hd.tmp)
	{
		hd.i = 0;
		while (hd.tmp->args[hd.i])
		{
			if (ft_strcmp(hd.tmp->args[hd.i], "<<") == 0)
			{
				save_fd(2);
				hd.file_name = handle_here_doc(shell, hd.tmp, &hd);
				if (hd.file_name == NULL)
					return (NULL);
				hd.tmp_n = make_heredoc_node(hd.file_name);
				free(hd.file_name);
				add_heredoc_node(&hd.here_docs, hd.tmp_n);
				hd.i++;
			}
			hd.i++;
		}
		hd.tmp = hd.tmp->next;
	}
	return (hd.here_docs);
}
