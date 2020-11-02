#include "minishell_bonus.h"

void		clean_quote(t_cmd *cmd)
{
	size_t	i;
	char	*tmp_av0;

	ft_printf("cmd->av[0] FIRST = %s\n", cmd->av[0]);
	i = 0;
	tmp_av0 = ft_strdup(cmd->av[0]);
	while (i < (size_t)cmd->ac)
	{
		if (is_clean(i, tmp_av0, cmd->av[i], cmd) == true)
			process_clean_command_quote(cmd, i);
		i++;
	}
	free(tmp_av0);
}

static int	clean_command(t_cmd *cmd)
{
	int			ret_command;

	ret_command = SUCCESS;
	ret_command = process_clean_command(cmd);
	if (ret_command != SUCCESS)
		return (ret_command);
	return (SUCCESS);
}

int			cleaner(t_cmd *cmd)
{
	clean_quote(cmd);
	//ft_printf("\n\033[0;32mDEBUG AV CLEAN QUOTE\n\033[0m");//DEBUG
	//debug_av(cmd->av, cmd->ac);//DEBUG
	return (clean_command(cmd));
}