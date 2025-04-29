/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 13:49:55 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/04/27 15:52:10 by mgamraou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
#define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
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



typedef enum
{
	TOKEN_WORD,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
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
	struct s_env	*next;
	struct s_env	*prev;
} t_env;
// init env in a stack
t_env	*init_env(char	**env);
void	add_node(t_env **env_list, t_env *new_n);
t_env	*new_node(char **variable);

t_token	*ft_add_token(t_token **token_list, char *value, token_type type);
bool validate_syntax(t_token *tokens);
bool	is_variable_assignment(const char *str);
t_env	*find_env_var(t_env *env_list, const char *var_name);
int	handle_assignment(t_env **env_list, const char *assignment);
char *get_env_value(t_env *env_list, const char *var_name);
t_token    *tokenize(const char *line);
t_command  *build_commands(t_token *tokens);
char *expand_input(char *input, int exit_status, t_env *env_list);
void        print_tokens(t_token *list);
void        print_commands(t_command *cmd);


char	*find_cmd_path(char *full_cmd, char **envp);
void	exec_cmd(char **args, char **envp, char **o_args, int has_pipe);
void	check_input(t_command *input, t_env *env_list, char **envp);
void	exec_builtin(char **arg, t_env *env_list, char **o_args);
int	is_builtin(char *arg);
void	clean_up(char *str, char **strs);
int	ft_cd(char **arg);
void	ft_echo(char **arg);
void	ft_pwd();
void	ft_env(t_env *env_list);
int	redirect_in(char **args);
char	**upd_env(t_env *env_list);
void	handle_pipeline(t_command *input, t_env *env_list, char **envp);
char	**get_cmd(char **o_args);
void	ft_unset(char **args, t_env **env_list);

#endif
