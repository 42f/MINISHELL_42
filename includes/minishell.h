/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:21:26 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:21:27 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef BONUS_FLAG
#  define BONUS_FLAG	FALSE
# endif

/*
*******************************_INCLUDE_**************************************
*/

# include "libft.h"

# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <dirent.h>

# include "line_editor.h"
# include "define.h"
# include "enum.h"
# include "struct.h"
# include "cleaner.h"
# include "lexer.h"

/*
*******************************_FUNCTION_*************************************
*/

int			exit_main(void);
t_data		*init_data_struct(void);
void		check_std_fd(void);
void		usage(int ac, char **av);
int			test(t_vector *input);
int			test_env(t_vector *input);
int			test_executor(t_list *jobs);

t_list		*test_lexer(t_vector *input);
int			test_parser(t_list *lexer_list);
t_list		*test_jobs(t_list *lexer_list);

/*
*******************************_PARSER_***************************************
*/

int			parser_token(t_list *token_list);
int			process_parser(t_list *list, t_valid_token *valid_token);
void		debug(const int type);

/*
*******************************_JOB/COMMAND_**********************************
*/

void		debug_jobs(t_list *job_list);
t_list		*get_jobs(t_list *token_list);
void		free_list_jobs(t_list **jobs);
void		debug_token_list(t_list *list);
int			next_is_end(t_list **token_list);
bool		is_cmd_sep(t_token *token);
bool		is_job_sep(t_token *token);
int			get_tablen(char **av);
int			add_cmd_to_job(t_job *job, t_cmd *cmd_model);
void		init_cmd_var(t_cmd *cmd, t_list **list);
int			fill_cmd_model(t_cmd *cmd, t_token *token, int type);
char		*debug_get_type(int type);
int			next_is_cmd_sep(t_list *token_list);
int			fill_name(t_token *token, t_cmd *cmd);
int			fill_ac(char **av);
int			verif_name(char *good, char *name);
int			verif_ac(int good, int ac);
int			verif_av(char **good, char **av, int ac);
int			verif_fd_string(char **good, char **fd_string);
int			verif_condition(int good, int condition);
int			verif_redirection(int good, int redirection);
void		free_list_job(t_list **job);
int			hub_test(int nb_job, int nb_command, t_cmd *cmd, int nb_test);
int			count_ac(t_list **list);
int			get_command(int type);
int			process_end_cmd(t_list *token_list, t_cmd *cmd, t_job *job);
int			process_add_command(t_token *token, t_cmd *cmd, t_list *token_list,
																	t_job *job);
int			is_cmd(t_token *token, t_cmd *cmd, int add_command);
int			is_end_cmd(t_token *token, t_list **token_list,
														t_cmd *cmd, t_job *job);
int			is_add_cmd(t_token *token, t_list *token_list, t_cmd *cmd,
																	t_job *job);
t_job		*init_job(void);
int			add_job_to_list(t_job *job, t_list **jobs, t_list *token_list,
																t_list **head);
int			count_assign(t_list **list);
int			count_exp(t_list **list);
int			fill_assign(int flag, int count_ac, t_cmd *cmd);
int			fill_exp(int flag, int count_ac, t_cmd *cmd);
void		create_cmd_fd_string(t_cmd *cmd, t_cmd *cmd_model);
int			resize_cmd(t_cmd *cmd, int count);
int			fill_data_cmd(t_token *token, t_cmd *cmd, int count);
void		debug_jobs(t_list *job_list);
void		process_between_both(char *str, t_vector *vct_good);
void		init_all(t_cmd *cmd);

/*
*******************************_EXECUTION_************************************
*/

void		process_open_file(t_cmd *cmd, int flag);
int			process_double_greater(char *str, t_cmd *cmd);
int			process_greater(char *str, t_cmd *cmd);
int			process_less(char *str, t_cmd *cmd);
void		print_file_error(char **str, size_t i, size_t size);
int			set_size(t_cmd *cmd, int flag);
void		export_envp(char **envp);
void		assign_envp_content(const t_cmd *command);
int			exec_builtin(t_cmd *command);
int			exec_binary(const t_cmd *command);
void		signal_manager(int set_mode);
int			execution_main_process(t_job *job, t_cmd *command,
													int p_in[2], int p_out[2]);

void		exec_subshell(t_job *job, t_cmd *cmd, int p_in[2], int p_out[2]);
void		executor(t_job *job);
char		*locate_binary_file(const char *bin_name);
int			is_path(const char *bin_name);

int			is_valid_job(const t_job *job);
int			is_last_cmd(const int cmd_index, const int nb_cmd);
int			is_solo_builtin(const size_t nb_cmd, t_cmd *command);
int			manage_subshell_exit_status(const int wstatus);
int			ms_dup(int old_fd);
int			ms_dup2(int old_fd, int new_fd);
int			ms_pipe(int pipe_fd[2]);
void		close_pipe_end(int pipe_to_close);
pid_t		fork_process(void);
void		dup_pipes(t_cmd *command, int p_in[2], int p_out[2]);
int			is_builtin(const t_cmd *command);

void		display_signal_str(int sig);
/*
*******************************_GENERAL_UTILES_*******************************
*/

t_data		*get_data_struct(t_data *mem);
t_vector	*safe_vct_new(void);
int			safe_vct_cpy(t_vector *dest, t_vector *src);
int			safe_vct_add(t_vector *vct, char c);
int			safe_vct_addstr(t_vector *vct, char *str);
int			safe_vct_addcharat(t_vector *vct, size_t index, char c);
void		free_char_arr(char **arr);

/*
*******************************_ERROR MANAGER_********************************
*/

void		print_set_errno(int errno_value, const char *err_str,
						const char *function_name, const char *error_source);
void		print_invalid_identifier(const char *function_name,
													const char *error_source);
void		print_invalid_option(const char *function_name,
								const char *error_source, const char *usage);

/*
*******************************_BUILTINS_*************************************
*/

int			set_builtin(int ac, char **av, char **envp);
int			history_builtin(int ac, char **av, char **envp);
int			exit_builtin(int ac, char **av, char **envp);
int			env_builtin(int argc, char **argv, char **envp);
bool		is_valid_export_identifier(char *id_to_test);
int			export_builtin(int argc, char **argv, char **envp);
int			unset_builtin(int argc, char **argv, char **envp);
int			pwd_builtin(int ac, char **av, char **envp);
int			echo_builtin(int ac, char **av, char **envp);
int			handle_pwd(char *dir);
int			cd_builtin(int ac, char **av, char **envp);
char		*handle_permission_denied(char *dir_denied);
int			check_cd_arg(int ac);
int			first_check(char *directory);
void		transform_new_dir(t_vector *new_dir, char *pwd, char *dir_denied);
int			check_arg(t_vector *vct_av, char c, char *av, int ac);
bool		is_long(t_vector *av, char c);
bool		parse_vct(t_vector *vct_av);
size_t		pop_arg(t_vector *av, int flag);
void		cut_arg(t_vector *vct_av, size_t len_before, size_t count_num,
							t_vector *av);
bool		is_numeric(t_vector *av);
int			process_error(t_vector *vct_home, char *dir, t_vector *vct_old_pwd);
int			print_error(t_vector *vct_av, char *av, char c, int flag);
void		handle_exit_value(t_vector *vct_av, t_vector *vct_av_cpy, char c);
int			handle_old_pwd(char *old_dir);
int			handle_permission_not(char *dir, char *pwd, char *old_dir);
void		swap_pwd(int flag, char *dir);
void		get_value(t_vector **vct_pwd, t_vector **vct_old,
														t_vector **vct_home);
void		free_clean_command(t_clean_cmd *clean_cmd, int flag);
void		set_env(t_vector *vct_pwd, t_vector *vct_old);
void		exit_error(t_vector *vct_av, char *av, char c, int flag);
void		set_old_pwd(char *dir, char *pwd, int flag);

/*
*******************************_ENV_MANAGER_**********************************
*/

void		free_env_list(t_list *env_lst);

void		del_env_elem(void *elem_content);
void		free_btree_node(t_btree *node);
void		unset_env(t_list *env_lst, const char *env_name);

void		store_env(t_list *env_lst, const char *env, int flags);
void		ms_setenv(t_list *env_lst, const char *env_name,
											const char *env_value, int flags);
void		ms_setenv_int(t_list *env_lst, const char *env_name, int value,
													int flags);
void		ms_putenv(t_list *env_lst, const char *env);
char		**get_envp(t_list *env_lst);
void		export_env(t_list *env_lst, const char *env);
int			is_special_environ(char *environ);
void		init_env(void);

void		print_env(t_list *env_lst);
void		add_to_btree(t_btree **tree, t_env *env);
void		print_export_output(t_list *env_lst, int flag);

void		put_env_name_setmode(char *env_name);
void		put_env_name_exportmode(char *env_name);
void		put_env_value(t_vector *env_value);

t_list		*get_env_list(t_list *mem);
t_env		*get_env_struct(t_list *env_lst, const char *env_name);
t_list		*get_env_node(t_list *env_lst, const char *env_name);
t_vector	*get_env_value_vct(t_list *env_lst, const char *env_name);
int			get_env_value_int(t_list *env_lst, const char *env_name);

void		parse_env(const char *env, char **env_name, char **env_value,
																int *overwrite);
void		parser_debug(t_token *token);

/*
*******************************_ENV_MANAGER_**********************************
*/

void		exit_routine(int err_code);
void		exit_routine_line_edition(t_le *le, int err_code);
void		exit_routine_env(void);

#endif
