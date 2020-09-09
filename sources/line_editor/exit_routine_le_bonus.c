#include "line_editor_bonus.h"

void	exit_routine_le(char *err_code)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &(get_env(GET)->termios_backup));
	if (err_code != NULL)
	{
		ft_putstr_fd(err_code, STDERR_FILENO);
		exit (FAILURE);
	}
	exit (0);
}
