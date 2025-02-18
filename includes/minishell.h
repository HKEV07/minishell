/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenaait <ibenaait@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 22:51:43 by hbouhach          #+#    #+#             */
/*   Updated: 2023/08/11 18:03:08 by ibenaait         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/errno.h>

# define RL_PROMPT "minishell$ "
# define NUMS "0123456789"
# define L_ALPHA "abcdefghijklmnopqrstuvwxyz"
# define U_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define ENV_KEY_CHARS \
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
# define CURRENT_DIR "."
# define ECHO_CMD "echo"
# define UNSET_CMD "unset"
# define ENV_CMD "env"
# define EXPORT_CMD "export"
# define EXIT_CMD "exit"
# define PWD_CMD "pwd"
# define CD_CMD "cd"
# define INT_LL 9223372036854775807
# define DEFAULT_PATH \
	"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."

typedef enum _token_type
{
	WORD,
	UNKNOWN,
	AMPERSAND = '&',
	WHITESPACE = ' ',
	TTAB = '\t',
	R_IN = '<',
	R_OUT = '>',
	PIPE = '|',
	PAR_OPEN = '(',
	PAR_CLOSE = ')',
	S_QUOTE = '\'',
	D_QUOTE = '"',
	DOLLAR = '$',
	WILDCARD = '*',
	APPEND = 0x3e3e,
	HEREDOC = 0x3c3c,
	NHEREDOC,
}							t_token_type;

typedef struct _env
{
	struct _env				*prev;
	char					*key;
	size_t					key_len;
	char					*value;
	size_t					value_len;
	int						skip;
	struct _env				*next;
}							t_env;

typedef struct _token
{
	struct _token			*prev;
	t_token_type			type;
	char					*token;
	int						ambiguous;
	struct _token			*next;
}							t_token;

typedef struct _out_redirection
{
	struct _out_redirection	*prev;
	char					*filename;
	int						append;
	int						ambiguous;
	struct _out_redirection	*next;
}							t_out_rediretion;

typedef struct _in_redirection
{
	struct _in_redirection	*prev;
	char					*filename;
	int						ambiguous;
	struct _in_redirection	*next;
}							t_in_rediretion;

typedef struct _heredoc
{
	struct _heredoc			*prev;
	char					*terminator;
	int						expand;
	struct _heredoc			*next;
}							t_heredoc;

typedef struct _command
{
	struct _command			*prev;
	char					*name;
	char					**argv;
	char					**envp;
	t_heredoc				*heredoc;
	t_in_rediretion			*in;
	t_out_rediretion		*out;
	t_token_type			last_redir;
	int						skip;
	int						exit_code;
	char					**(*get_envp)(struct _command *self);
	void					(*set_exit_code)(struct _command *self, \
								int exit_code);
	struct _command			*next;
}							t_command;

typedef struct s_pipex
{
	int						infile;
	int						inf_here;
	int						outfile;
	pid_t					pr;
	int						*pipend;
	int						index;
	int						nbrcmd;
}							t_pipex;

typedef struct _state
{
	t_env					*g_env;
	char					*pwd;
	int						is_sigint;
	int						duped_stdin;
	int						special_sigint;
}							t_state;

t_state						g_state;

char						*ft_substr(char const *s, unsigned int start,
								size_t len);
char						**ft_split(char *s);
char						*spacify(char *cmd);
size_t						ft_strlen(const char *s);
size_t						find_next(char const *str, char c);
size_t						ft_strncpy(char *dst, char *src, size_t len);
t_env						*build_env(char *envp[]);
size_t						get_index_of(char *str, char c);
void						*_calloc(size_t __size);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
t_env						*get_env_by_key(char *key, size_t len);
size_t						get_key_len(char *e);
int							char_in_str(char *str, char c);
size_t						find_next_diff(char *str, char c);
size_t						find_next_until(char *str, char c, size_t end);
t_token						*populate_tokens(char *tokens[]);
size_t						count_c_until_diff(char *str, char c);
int							expand(t_token *tokens);
int							syntax_error(t_token *tokens);
char						*ft_strdup(const char *s1);
t_token						*new_token_node(t_token *prev, char *token,
								t_token_type type, t_token *next);
t_token						*wildcard(t_token *tokens);
void						*inject_new_tokens(t_token *token, char *tokens[]);
char						**env_to_envp(void);
t_command					*build_command(t_token *tokens);
char						*get_type(t_token_type t);
char						*remove_quotes(char *str);
void						delete_env_by_key(char *key, size_t len);
char						*expand_str(char *str, int quotes);
char						**get_envp(t_command *self);
void						free_all(t_token *tokens, t_command *cmd);
t_token						*map_str_to_token(t_token *tokens, char *str);
void						set_exit_code(t_command *self, int exit_code);
int							is_builtin(char *cmd_name);
void						handle_builtin(t_command *cmd, int fd);
t_token_type				get_token_type(char *token, char *next);
char						*get_key(char *e);
char						*get_value(char *e);
t_env						*env_pushback(t_env **head);
int							should_skip_env(t_env *e);
void						destroy_tokens(t_token *tokens);
void						sort_keys(char **keys, size_t size);
char						*get_env_path(void);
int							invalid_filename_expansion(char *token);
size_t						count_wd_results(char *match);
int							is_not_cmd(char *cmd);
t_env						*add_default_env(t_env **env, char *e);
int							is_in_quotes(char *str, size_t pos);
int							does_it_match(char *entry, char *match);
size_t						wd_keys_count(char **v);
int							is_wildcard(char *str);
int							wd_should_skip(char *match, char *entry);
void						remove_d_q(char *str);
int							handle_sq(char *str, char *new_str, size_t *j,
								size_t *i);
size_t						get_expanded_len(char *cmd, int quotes);
t_heredoc					*heredoc_pushback(t_heredoc **head, t_token *token);
t_command					*redir_pushback(void **head, t_token_type type);
t_command					*command_pushback(t_command **head);
char						**add_argv(t_command *cmd, t_token **token);
void						cmd_check_and_move(t_command *cmd,
								t_token **tokens);
char						**copy_old_argv(char **ov, size_t *count);
char						**copy_old_argv(char **ov, size_t *count);
void						*add_redir(t_command *cmd, t_token *token);
int							create_new_cmd(t_command **cmd,
								t_command **cmd_head);
char						**get_new_argv(char *cmd_name);
void						check_for_invalid_rs(t_token *tokens);
int							rin_check(char *filename);
int							is_redir(t_token_type type);
int							invalid_redir(char *filename, char *original,
								t_token_type type);
void						remove_all_following_rs(t_token **t,
								t_token *start);
int							rin_check(char *filename);
int							rout_check(char *filename);
t_token						*token_pushback(t_token **head, char *token,
								t_token_type type);
int							check_q_error(char *str);
int							invalid_key(char *k, char *e, int *code,
								int *append);
int							valid_request(char *e, int *code);
void						_export(char *e, int *code);

char						*ft_itoa(int n);
long long					ft_atoi(const char *str);
char						**_ft_split(char const *s, char c);
char						*ft_substr(char const *s, unsigned int start,
								size_t len);
char						*ft_strjoin(char const *s1, char const *s2);
void						*ft_memmove(void *dst, const void *src, size_t len);
size_t						ft_strlen(char const *s);
char						*ft_find_path(char *cmd);
int							ft_redirection_out(t_out_rediretion *out_red);
int							ft_redirection_in(t_in_rediretion *in_red);
int							ft_herdoc(t_heredoc *here);
int							ft_open_here_doc(char *del, int expand, int fd);
void						ft_duplicate(int out, int in);
int							ft_check_here_doc(t_command *cmd);
void						ft_m_cmd(t_command *cmd, t_pipex *ppx);
void						ft_first_cmd(t_command *cmd, t_pipex *ppx);
void						ft_last_cmd(t_command *cmd, t_pipex *ppx);
void						ft_excute_cmd(t_command *cmd);
void						ft_pipe(t_command *cmd, t_pipex *ppx);
int							ft_nbrcmd(t_command *cmd);
void						ft_close_in_out(t_pipex *ppx);
void						ft_execve_cmd(t_command *cmd);
void						ft_close_pipe(t_pipex *ppx);
void						ft_creat_pipe(t_pipex *ppx);
int							ft_read_from_file(void);
void						handle_exit(t_command *cmd, int pid);
int							is_ambg(t_command *cmd);
void						restore_stdin(void);
t_env						*env_add_shlvl(void);
char						**ft_split_cleanup(char **wordsv, size_t max);
int							is_whitespace(char c);
int							all_empty(char *str);
void						destroy_r_in(t_in_rediretion *r_in,
								t_token *tokens);
void						destroy_r_out(t_out_rediretion *r_out,
								t_token *tokens);
void						update_signals_props(t_command *cmd);
int							is_del(char *del, char *bufer);

void						pwd(t_command *cmd, int fd);
void						echo(t_command *cmd, int fd);
void						env_cmd(t_command *cmd, int fd);
void						unset(t_command *cmd);
void						export(t_command *cmd, int fd);
void						exit_cmd(t_command *cmd);
int							cd(t_command *cmd);
int							handle_double_point(t_command *cmd);
int							check_dir(t_command *cmd, char *path);
int							handle_home(t_command *cmd, int i);
char						*get_env_path_home(void);
void						print_mes(t_command *cmd, char *str, int i);
int							is_dir(char *filename);
void						set_oldpwd(char *oldpwd);
void						set_pwd(char *pwd);
int							ft_sin_her(t_command *cmd, t_pipex *ppx);
int							ft_open_red(t_command *cmd, t_pipex *ppx);
void						ft_dup_in_out(t_command *cmd, t_pipex *ppx);

void						handle_sigint_idle(int sig);
void						handle_signint_exec(int signum);
void						handle_sigquit(int signum);
#endif
