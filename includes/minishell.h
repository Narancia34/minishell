/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 13:49:55 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/06/12 11:56:16 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
#define MINISHELL_H

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

static volatile sig_atomic_t	g_signal_flag = 0;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_type;

typedef enum
{
	TOKEN_WORD,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_ASSIGNMENT
} token_type;

typedef struct s_token
{
	char			*value;
	token_type		type;
	struct s_token	*next;
} t_token;

typedef struct s_command
{
	char				**args;
	token_type			type;
	int					arg_size;
	struct	s_command	*next;
} t_command;

// exec part

typedef struct s_env
{
	char	*var_name;
	char	*var_value;
	int		exported;
	int		flag;
	struct s_env	*next;
	struct s_env	*prev;
} t_env;

typedef struct s_var
{
	char	*var_name;
	char	*var_value;
	struct s_var	*next;
} t_var;

typedef struct s_shell
{
	t_env	*env_list;
	t_var	*var_list;
} t_shell;

typedef struct s_pid
{
	pid_t	pid;
	struct s_pid	*next;
} t_pid;

typedef struct s_here_docs
{
	char	*file_name;
	struct s_here_docs	*next;
} t_here_docs;

void check_and_set_assignment(t_token *token);
// init env in a stack
t_env	*init_env(char	**env);
void	add_node(t_env **env_list, t_env *new_n);
t_env	*new_node(char **variable);
void	free_tokens(t_token *head);

//expander
t_env	*find_env_var(t_env *env_list, const char *var_name);
char	*strjoin_and_free(char *s1, char *s2);
char	*expand_env_vars(char *input, int exit_status, t_env *env_list);
char	*remove_quotes(char *input);
bool	is_variable_assignment(char *str);
char	*get_var_list(t_var	*var_list, const char	*var_name);
bool is_valid_var_char(char c, bool first_char);
char	*expand_input(char *input, int exit_status, t_env *env_list);
char	*append_character_as_is(const char **start, char *result);
void	handle_env_var(t_env *env_list, char **result, const char **start);
char	*expand_tilde(char *input);
char	*get_env_value(t_env *env_list, const char *var_name);

//tokens
t_token		*ft_add_token(t_token **token_list, char *value, token_type type);
t_token	*tokenize(const char *input);
t_command	*build_commands(t_token *tokens);
void	set_size(t_command *head);
void	set_type(t_command *head);
char *read_quoted(const char *input, int *i);
char	**ft_realloc(char *arg, char **old_arr);
char *read_operator(const char *str, int *i);
char *read_word(const char *str, int *i);
token_type	get_operation_type(const char *op);

//check syntax
bool	validate_syntax(t_token *tokens);

//assignment
int		handle_assignment(t_env **env_list, const char *assignment);
void	print_commands(t_command *cmd);   // Optional for debug
void	print_tokens(t_token *list);      // Optional for debug

//execution part
char	*find_cmd_path(char *full_cmd, char **envp);
void	exec_cmd(char **args, char **envp, char **o_args, int has_pipe, int *exit_s, t_env **env_list, t_command *input, t_here_docs *here_docs);
void	exec_piped_cmd(char **args, char **envp, char **o_args, t_env **env_list, t_command *input, t_pid *pid_list, t_here_docs *here_docs);
int	check_input(t_command *input, t_env **env_list, char **envp, int *exit_s);
int	exec_builtin(char **arg, t_env **env_list, char **o_args, int *exit_s, t_here_docs *here_docs);
int	is_builtin(char *arg);
void	clean_up(char *str, char **strs);
int	ft_cd(char **arg, t_env **env_list);
int	ft_echo(char **arg);
int	ft_pwd();
int	ft_env(t_env *env_list);
int	redirect_in(char **args, t_env *env_list, t_here_docs *here_docs);
char	**upd_env(t_env *env_list);
void	handle_pipeline(t_command *input, t_env **env_list, char **envp, int *exit_s, t_here_docs *here_doc);
char	**get_cmd(char **o_args);
int	ft_unset(char **args, t_env **env_list);
void	handle_var(t_var **var_list, char *arg);
void	add_to_list(t_env **env_list, t_env *new_n);
t_env	*make_node(char *var_name, char *var_value, int flag);
int	ft_export(char **args, t_env **env_list);
int	lstlen(t_env *lst);
void	handle_shlvl(t_env **env_list);
void	setup_signals();
void	ignore_signals();
t_pid	*make_pid_node(pid_t pid);
void	add_pid_node(t_pid **pid_list, t_pid *new_n);
void free_commands(t_command *cmd);
char	*expander_heredoc(char	*input, t_env *env_list);
void	save_fd(int flag);
t_here_docs	*here_doc(t_command *input, int *exit_s, t_env *env_list, char **env);
void	free_env(t_env **env_list);
void	free_pids(t_pid *pid_list);
void	free_here_docs(t_here_docs *here_docs);


#endif
