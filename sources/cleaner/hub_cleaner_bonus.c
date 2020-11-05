#include "minishell_bonus.h"

/////////////////////////////////////////////////////////////////////////////////
/*PROCESS_REDIRECTION A AJOUTER AU DEBUT DE L'EXECUTOR*/

static int process_less(char *str, t_cmd *cmd)
{
	int fd;
	static size_t i = 0;

	fd = open(str, O_RDONLY | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP |
				S_IROTH);
	if (fd < 0)
	{
		if (cmd->tmp_fd_in > 2)
			close(cmd->tmp_fd_in);
		if (cmd->tmp_fd_out > 2)
			close(cmd->tmp_fd_out);
		print_set_errno(errno, "bash", str);
		return (FAILURE);
	}
	if (cmd->tmp_fd_in > 2)
		close(cmd->tmp_fd_in);
	cmd->tmp_fd_in = fd;
	cmd->fd[STDIN_FILENO] = fd;
	i++;
	return (SUCCESS);
}

static int process_greater(char *str, t_cmd *cmd)
{
	int fd;
	static size_t i = 0;

	fd = open(str, O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR |
				S_IRGRP | S_IROTH);
	if (fd < 0)
	{
		if (cmd->tmp_fd_in > 2)
			close(cmd->tmp_fd_in);
		if (cmd->tmp_fd_out > 2)
			close(cmd->tmp_fd_out);
		print_set_errno(errno, "bash", str);
		return (FAILURE);
	}
	if (cmd->tmp_fd_out > 2)
		close(cmd->tmp_fd_out);
	cmd->tmp_fd_out = fd;
	cmd->fd[STDOUT_FILENO] = fd;
	i++;
	return (SUCCESS);
}
static int process_double_greater(char *str, t_cmd *cmd)
{
	int fd;
	static size_t i = 0;

	fd = open(str, O_RDONLY | O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR |
				S_IRGRP | S_IROTH);
	if (fd < 0)
	{
		if (cmd->tmp_fd_in > 2)
			close(cmd->tmp_fd_in);
		if (cmd->tmp_fd_out > 2)
			close(cmd->tmp_fd_out);
		print_set_errno(errno, "bash", str);
		return (FAILURE);
	}
	if (cmd->tmp_fd_append > 2)
		close(cmd->tmp_fd_append);
	cmd->tmp_fd_append = fd;
	cmd->fd[STDOUT_FILENO] = fd;
	i++;
	return (SUCCESS);
}

static void	init_cmd_redirection(t_cmd *cmd)
{
	if (cmd->fd[0] != STDIN_FILENO && cmd->fd[0] == cmd->tmp_fd_in)
		cmd->redirection = cmd->redirection | F_REDIRECT_IN;
	if (cmd->fd[1] != STDOUT_FILENO && cmd->fd[1] == cmd->tmp_fd_out)
		cmd->redirection = cmd->redirection | F_REDIRECT_OUT;
	if (cmd->fd[1] != STDOUT_FILENO && cmd->fd[1] == cmd->tmp_fd_append)
		cmd->redirection = cmd->redirection | F_REDIRECT_OUT |
						   F_REDIRECT_OUT_APPEND;
}

static void process_open_file(t_cmd *cmd)
{
	size_t i;
	int ret_file;

	i = 0;
	ret_file = SUCCESS;
	while (i < cmd->count_redir)
	{
		if (ft_strequ(cmd->tab_redir[i], DOUBLE_GREATER) == TRUE)
			ret_file = process_double_greater(cmd->tab_redir[i + 1], cmd);
		else if (ft_strequ(cmd->tab_redir[i], GREATER_THAN) == TRUE)
			ret_file = process_greater(cmd->tab_redir[i + 1], cmd);
		else if (ft_strequ(cmd->tab_redir[i], LESS_THAN) == TRUE)
			ret_file = process_less(cmd->tab_redir[i + 1], cmd);
		if (ret_file == FAILURE)
			cmd->redirection = cmd->redirection | F_REDIRECT_FAILURE;
		i += 2;
		if (i >= cmd->count_redir)
			break;
	}
	init_cmd_redirection(cmd);
}

/////////////////////////////////////////////////////////////////////////////////

static void executor(t_job *job)
{
}

static int process_cleaner(t_cmd *cmd)
{
	int ret_cleaner;

	ret_cleaner = cleaner(cmd);
	if (ret_cleaner != SUCCESS)
		return (ret_cleaner);
	debug_cleaner(cmd);
	return (SUCCESS);
}

int hub_cleaner(t_list *job_list)
{
	t_job *job;
	t_list *tmp_cmd_lst;
	t_cmd *cmd;
	int ret_cleaner;

	ret_cleaner = SUCCESS;
	while (job_list != NULL)
	{
		job = job_list->content;
		tmp_cmd_lst = job->cmd_lst;
		while (tmp_cmd_lst != NULL)
		{
			cmd = tmp_cmd_lst->content;
			ret_cleaner = process_cleaner(cmd);
			if (ret_cleaner != SUCCESS)
				return (ret_cleaner);
			ft_printf("\033[0;32mDEBUG FD FINAL\n\033[0m"); //DEBUG
			debug_fd(cmd->fd);
			ft_printf("\n");
			ft_printf("\033[0;32mDEBUG REDIR FINAL\n\033[0m");//DEBUG
			debug_redir(cmd->tab_redir, cmd->count_redir);
			tmp_cmd_lst = tmp_cmd_lst->next;
		}
		executor(job); //UTILISER LE VRAI EXECUTOR
		job_list = job_list->next;
	}
	return (SUCCESS);
}