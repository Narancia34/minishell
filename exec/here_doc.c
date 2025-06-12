/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgamraou <mgamraou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 10:26:23 by mgamraou          #+#    #+#             */
/*   Updated: 2025/06/12 11:58:24 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*make_file_name()
{
	char	*res;
	int		fd;
	int		i;
	ssize_t	b;

	res = malloc(sizeof(char) * 10);
	if (!res)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	b = read(fd, res, 9);
	if (b == -1)
		perror("read()");
	res[9] = '\0';
	close(fd);
	i = 0;
	while (res[i])
	{
		res[i] = (res[i] % 26) + 'a';
		i++;
	}
	return (res);
}

t_here_docs	*make_heredoc_node(char *file_name)
{
	t_here_docs	*node;

	node = malloc(sizeof(t_here_docs));
	if (!node)
		return (NULL);
	node->file_name = ft_strdup(file_name);
	node->next = NULL;
	return (node);
}

void	add_heredoc_node(t_here_docs **here_docs, t_here_docs *new_n)
{
	t_here_docs	*current;

	if (!*here_docs)
	{
		*here_docs = new_n;
		return ;
	}
	current = *here_docs;
	while (current->next)
		current = current->next;
	current->next = new_n;
}

char	*handle_here_doc(char *delimiter, t_env *env_list, int *exit_s, t_command *input, char **env, t_here_docs *here_docs)
{
	char	*line;
	char	*res;
	char	*tmp;
	char	*file_name;
	char	*expanded;
	int		fd;
	pid_t	pid;
	int	status;

	ignore_signals();
	file_name = make_file_name();
	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_DFL);
		res = ft_strdup("");
		while (1)
		{
			line = readline(">");
			if (!line)
			{
				ft_putstr_fd("warining: here-document delimited by end-of-file\n", 2);
				break ;
			}
			if (ft_strcmp(line, delimiter) == 0)
				break ;
			tmp = line;
			line = ft_strjoin(line, "\n");
			free(tmp);
			tmp = res;
			res = ft_strjoin(res, line);
			free (tmp);
			free (line);
		}
		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("couldnt open here-document\n", fd);
			return (NULL);
		}
		expanded = expander_heredoc(res, env_list);
		ft_putstr_fd(expanded, fd);
		free(expanded);
		free(res);
		close(fd);
		(void)env;
		free_env(&env_list);
		free_commands(input);
		clean_up(file_name, env);
		free_here_docs(here_docs);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*exit_s = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
	*exit_s = 130;
	printf("\n");
	unlink(file_name);
	free(file_name);
	setup_signals();
	g_signal_flag = WTERMSIG(status);
	return (NULL);
}
	setup_signals();
	return (file_name);
}

t_here_docs	*here_doc(t_command *input, int *exit_s, t_env *env_list, char	**env)
{
	t_command	*tmp;
	t_here_docs	*tmp_n;
	char	*file_name;
	int	i;
	t_here_docs	*here_docs;

	(void)exit_s;
	here_docs = NULL;
	tmp = input;
	while (tmp)
	{
		i = 0;
		while (tmp->args[i])
		{
			if (ft_strcmp(tmp->args[i], "<<") == 0)
			{
				save_fd(2);
				file_name = handle_here_doc(tmp->args[i+1], env_list, exit_s, input, env, here_docs);
				if (file_name == NULL)
					return (NULL);
				tmp_n = make_heredoc_node(file_name);
				free(file_name);
				add_heredoc_node(&here_docs, tmp_n);
				i++;
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (here_docs);
}
