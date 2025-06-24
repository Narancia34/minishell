/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 13:49:55 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/24 14:24:36 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "../libft/libft.h"

extern volatile sig_atomic_t	g_signal_flag;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ASSIGNMENT
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				is_single_quoted;
	int				is_double_quoted;
	int				flag;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**args;
	t_token_type		type;
	int					arg_size;
	int					flag;
	struct s_command	*next;
}	t_command;

typedef struct s_env
{
	char			*var_name;
	char			*var_value;
	int				exported;
	int				flag;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_var
{
	char			*var_name;
	char			*var_value;
	struct s_var	*next;
}	t_var;

typedef struct s_pid
{
	pid_t			pid;
	struct s_pid	*next;
}	t_pid;

typedef struct s_here_docs
{
	char				*file_name;
	struct s_here_docs	*next;
}	t_here_docs;

typedef struct s_pwd
{
	int	oldpwd;
	int	pwd;
}	t_pwd;

typedef struct s_check
{
	t_command	*tmp;
	char		**args;
	t_here_docs	*here_docs;
	int			ret;
}	t_check;

typedef struct s_export
{
	char	*var_name;
	char	*var_value;
	t_env	*new;
	int		i;
	int		j;
	int		flag;
	t_env	*tmp;
}	t_export;

typedef struct s_red
{
	char	**args;
	int		i;
	int		j;
	int		red;
}	t_red;

typedef struct s_path
{
	char		*res;
	char		*pre_path;
	char		**pre_paths;
	struct stat	sb;
}	t_path;

typedef struct s_redirection
{
	int	fd_in;
	int	fd_out;
	int	i;
}	t_redirection;

typedef struct s_pipeline
{
	int			fd[2];
	int			prev_fd;
	pid_t		pid;
	int			status;
	t_command	*tmp;
	char		**args;
	t_pid		*tmp_n;
	t_pid		*to_free;
	int			count;
	t_here_docs	*here_docs_head;
	t_here_docs	*saved_head;
}	t_pipeline;

typedef struct s_hd
{
	t_command	*tmp;
	t_here_docs	*tmp_n;
	t_here_docs	*here_docs;
	char		*file_name;
	int			i;
}	t_hd;

typedef struct s_here_d
{
	char	*line;
	char	*res;
	char	*tmp;
	char	*file_name;
	char	*expanded;
	int		fd;
	pid_t	pid;
	int		status;
}	t_here_d;

typedef struct s_shell
{
	t_command	*input;
	t_token		*tokens;
	t_pid		*pid_list;
	char		*rl;
	t_env		*env_list;
	char		**envp;
	int			exit_s;
}	t_shell;

void			check_and_set_assignment(t_token *token);
t_env			*init_env(char	**env);
void			add_node(t_env **env_list, t_env *new_n);
t_env			*new_node(char **variable);
void			free_tokens(t_token *head);
t_env			*find_env_var(t_env *env_list, const char *var_name);
char			*strjoin_and_free(char *s1, char *s2);
void			expand_tokens(t_token *tokens,
					int exit_status, t_env *env_list);
char			*expand_env_vars(char *input,
					int exit_status, t_env *env_list, int do_expand);
char			*remove_quotes(char *input);
bool			is_variable_assignment(char *str);
char			*get_var_list(t_var	*var_list, const char	*var_name);
bool			is_valid_var_char(char c, bool first_char);
char			*expand_input(char *input, int exit_status, t_env *env_list);
char			*append_character_as_is(const char **start, char *result);
void			handle_env_var(t_env *env_list,
					char **result, const char **start);
char			*expand_tilde(char *input);
char			*get_env_value(t_env *env_list, const char *var_name);
t_token			*ft_add_token(t_token **token_list,
					char *value, t_token_type type, int *quote);
t_token			*tokenize(const char *input);
t_command		*build_commands(t_token *tokens);
int				append_arg(t_command *cmd, const char *value);
void			free_single_command(t_command *cmd);
void			free_cmd(t_command *cmd);
void			set_size(t_command *head);
void			set_type(t_command *head);
char			*read_quoted(const char *input, int *i);
char			**ft_realloc(char *arg, char **old_arr);
char			*read_operator(const char *str, int *i);
char			*read_word(const char *str, int *i, int *quote);
t_token_type	get_operation_type(const char *op);
bool			validate_syntax(t_token *tokens);
int				handle_assignment(t_env **env_list, const char *assignment);
void			print_commands(t_command *cmd);
void			print_tokens(t_token *list);
char			*find_cmd_path(char *full_cmd, char **envp, int *exit_s);
void			exec_cmd(t_shell *shell, char **args,
					char **o_args, t_here_docs *here_docs);
void			exec_piped_cmd(t_shell *shell, char **args,
					char **o_args, t_here_docs *here_docs);
int				check_input(t_shell *shell);
int				exec_builtin(t_shell *shell, char **arg,
					char **o_args, t_here_docs *here_docs);
int				is_builtin(char *arg);
void			clean_up(char *str, char **strs);
int				ft_cd(char **arg, t_env **env_list);
int				ft_echo(char **arg);
int				ft_pwd(void);
int				ft_env(t_env *env_list);
int				redirect(char **args, t_here_docs *here_docs);
char			**upd_env(t_env *env_list);
void			handle_pipeline(t_shell *shell, t_here_docs *here_docs);
char			**get_cmd(char **o_args);
int				ft_unset(char **args, t_env **env_list);
void			handle_var(t_var **var_list, char *arg);
void			add_to_list(t_env **env_list, t_env *new_n);
t_env			*make_node(char *var_name, char *var_value, int flag);
int				ft_export(char **args, t_env **env_list);
int				lstlen(t_env *lst);
void			handle_shlvl(t_env **env_list);
void			setup_signals(void);
void			ignore_signals(void);
t_pid			*make_pid_node(pid_t pid);
void			add_pid_node(t_pid **pid_list, t_pid *new_n);
void			free_commands(t_command *cmd);
char			*expander_heredoc(char	*input, t_env *env_list);
void			save_fd(int flag);
t_here_docs		*here_doc(t_shell *shell);
void			free_env(t_env **env_list);
void			free_pids(t_pid *pid_list);
void			free_here_docs(t_here_docs *here_docs);
int				ft_exit(char **arg);
void			change_pwd_env(char *old_dir,
					char *current_dir, t_env **env_list);
int				add_to_env(t_env **env_list, char *arg);
int				has_heredoc(t_command *input);
int				has_pipe(t_command *input);
void			handle_exec(char *path, char **args, t_shell *shell);
void			free_all(t_shell *shell, char **args);
char			*cat_path_cmd(char *pre_path, char *full_cmd);
char			*find_pre_path(char **envp);
int				read_from_heredoc(t_here_docs *here_docs);
void			collect_exit_s(int status, t_shell *shell);
void			handle_pipe_util_a(int prev_fd, int *fd, t_command *tmp);
void			handle_pipe_util_b(int *prev_fd, int*fd);
int				count_here_docs(char	**cmd);
void			handle_child_p(t_shell *shell,
					t_here_docs *here_docs, t_pipeline *pipeline);
void			handle_pipe_util_c(t_shell *shell,
					t_pipeline *pipeline, t_here_docs **here_docs);
void			wait_for_pipes(t_shell *shell, t_pipeline *pipeline);
char			*make_file_name(void);
t_here_docs		*make_heredoc_node(char *file_name);
void			add_heredoc_node(t_here_docs **here_docs, t_here_docs *new_n);

#endif
