#include "minishell_bonus.h"

static void		exec_subshell(const t_cmd *command, int p_in[2], int p_out[2])
{
	pid_t	pid;
	
	pid = fork_process();
	if (pid == FAILURE) 						//implement error managment
		return ;
	else if (pid == 0)
	{
		dup_pipes(command, p_in, p_out);
		if (is_builtin(command) == TRUE)
			exec_builtin(command);
		else	
			exec_binary(command);
		exit(42);											 //to change
	}
	else if (pid != 0 && pid != FAILURE)
		ms_setenv_int(get_env_list(GET), "!", (int)pid, F_OVERWRITE);
}
static int		execution_process(const t_cmd *command, const int nb_cmd,
												int p_in[2], int p_out[2])
{
//		if (nb_cmd == 1 && command->name == NULL && command->env != NULL)
			//DO ASSIGNATIONS export_execution_context_env(command);
//	else if (ft_strequ(command->name, "exit") == TRUE)
	if (ft_strequ(command->name, "exit") == TRUE)
		exec_builtin(command);
	else if (nb_cmd == 1 && is_builtin(command) == TRUE && command->redirection == 0)
		exec_builtin(command);
	else
		exec_subshell(command, p_in, p_out);
	if (command->redirection > 0)
		close(command->fd[STDOUT_FILENO]);
	return (21);
}

static void		do_pipe(int pipe_fd[2])
{
	int		pipe_ret;

	pipe_ret = pipe(pipe_fd);
	ft_printf("Just PIPED : [%d, %d]\n", pipe_fd[0], pipe_fd[1]);
	if (pipe_ret == FAILURE)
		print_set_errno(errno, NULL, "do pipe: ", NULL);
}

void			executor(const t_job *job)
{
	size_t	i;
	int		ret_value;
	t_list	*cmd_cursor;
	int		p_in[2];
	int		p_out[2];
	pid_t	pid;
	int		wstatus;
	
	if (job == NULL || job->cmd_lst == NULL)
		return ;
	i = 0;
	ft_memset(p_in, UNSET, sizeof(int[2]));
	ft_memset(p_out, UNSET, sizeof(int[2]));
	cmd_cursor = job->cmd_lst;
	while (i < job->nb_cmd && cmd_cursor->content != NULL)
	{
		ret_value = 0;
		if (i < job->nb_cmd - 1 || ((t_cmd *)cmd_cursor->content)->redirection == 1)
			do_pipe(p_out);
		else
			ft_memset(p_out, UNSET, sizeof(int[2]));
system("ls -la /proc/$$/fd ; echo \"\n\"");
		execution_process(cmd_cursor->content, job->nb_cmd, p_in, p_out);
		close_pipe_end(p_in[R_END]);
		close_pipe_end(p_in[W_END]);
		if (i < job->nb_cmd - 1)
			ft_memmove(p_in, p_out, sizeof(int[2]));
		else
		{
			close_pipe_end(p_out[R_END]);
			close_pipe_end(p_out[W_END]);
		}
		cmd_cursor = cmd_cursor->next;
		pid = wait(&wstatus);
		i++;
	}
	ms_setenv_int(get_env_list(GET), "?", ret_value, F_OVERWRITE);
	ft_printf("\n(exec) $? = %d\n", get_env_value_int(get_env_list(GET), "?"));
	manage_exit_status(wstatus, pid);
}


/*	
	if (ret_value != 0 || errno != 0)
		ret_value = 128 + ret_value;
*/
