#include "minishell_bonus.h"

static void	newprompt_at_signal(int sig)
{
	(void)sig;
	ft_putstr_fd("^C\n", STDOUT_FILENO);
	init_prompt();
	ms_setenv_int(get_env_list(GET), "?", 130, F_OVERWRITE);
}

static void	exit_at_signal(int sig)
{
	(void)sig;
	exit_routine_le("exit");
}

static void	window_at_signal(int sig)
{
	(void)sig;
	update_window_size();
}

void		signal_manager(int set_mode)
{
	int					i;
	static const int	sig_nb = 5;
	static const int	sig_list[sig_nb] = {
		SIGHUP, SIGINT, SIGQUIT,
		SIGTERM, SIGWINCH	};
	static void			(*handlers_mode_cmd_line[sig_nb])(int) = {
		exit_at_signal, newprompt_at_signal, SIG_IGN,
		exit_at_signal, window_at_signal	};

	i = 0;
	while (i < sig_nb)
	{
		if (set_mode == SIG_MODE_DEFAULT)
			signal(sig_list[i], SIG_DFL);
		else if (set_mode == SIG_MODE_EXEC)
			signal(sig_list[i], SIG_IGN);
		else if (set_mode == SIG_MODE_CMD_LINE)
			signal(sig_list[i], handlers_mode_cmd_line[i]);
		i++;
	}
}
