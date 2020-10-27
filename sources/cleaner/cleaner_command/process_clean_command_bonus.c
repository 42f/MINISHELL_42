#include "minishell_bonus.h"

static t_clean_cmd	*init_clean_command()
{
	t_clean_cmd	*clean_cmd;
	
	clean_cmd = (t_clean_cmd *)malloc(sizeof(t_clean_cmd));
	if (clean_cmd == NULL)
		return (NULL);
	clean_cmd->count_assign = 0;
	clean_cmd->ac = 0;
	clean_cmd->index_export = 0;
	clean_cmd->av = NULL;
	return (clean_cmd);
}

static int	process_command_any(t_cmd *cmd, int ass_or_exp, bool is_bad,
								enum e_cmd cmd_type)
{
	if (is_bad == false)
		ft_printf("EXPORT POP ENV ET ASSIGN\n");
	//POP DANS ENV ET ASSIGN
	else if (is_bad = true)
		ft_printf("POP ENV CELUI FAUX\n");
	//POP SAUF CELUI FAUX
}

static int process_command_command(t_cmd *cmd, int ass_or_exp, bool is_bad,
									enum e_cmd cmd_type)
{
	//SI LA COMMANDE EN QUESTION EST UNSET -->> ASS_OR_EXP = FALSE
	ft_printf("EXPORT POP MAIS PAS DANS ENV\n");
}

static int process_command_export(t_cmd *cmd, int ass_or_exp, bool is_bad,
								   enum e_cmd cmd_type)
{
	t_clean_cmd	*clean_cmd;

	iter_clean_quote(cmd, (size_t)cmd->ac);
	clean_cmd = init_clean_command();
	if (clean_cmd == NULL)
	{
		ft_printf("CLEAN CMD == NULL\n");
		free_clean_command(clean_cmd, NOT_ALL_FREE);
		return (FAILURE);
	}
	count_ac_assign(cmd, clean_cmd, is_bad);
	clean_cmd->ac = cmd->ac - clean_cmd->count_assign;
	if (init_tab_assign_ac(clean_cmd, cmd) == FAILURE)
	{
		ft_printf("INIT TAB ASSIGN AC == NULL\n");
		free_clean_command(clean_cmd, ALL_FREE);
		return (FAILURE);
	}
	if (fill_clean_cmd(cmd, clean_cmd) == FAILURE)
	{
		ft_printf("FILL CLEAN CMD == NULL\n");
		free_clean_command(clean_cmd, ALL_FREE);
		return (FAILURE);
	}
	free_clean_command(clean_cmd, ALL_FREE);
	return (SUCCESS);
}

int process_clean_command(t_cmd *cmd, int ass_or_exp, bool is_bad,
						  enum e_cmd cmd_type)
{
	//COMPARER CHAQUE IF AVEC LES INDEXS DE OU SE TROUVE LES ASSIGN
	// + revoir les diff printf
	if (cmd_type == E_EXPORT_EXEC || cmd_type == E_EXPORT_NO_EXEC)
	{
		if (process_command_export(cmd, ass_or_exp, is_bad, cmd_type) == FAILURE)
			return (FAILURE);
	}
	if (cmd_type == E_COMMAND)
	{
		if (process_command_command(cmd, ass_or_exp, is_bad, cmd_type) == FAILURE)
			return (FAILURE);
	}
	if (cmd_type == E_ANY)
	{
		if (process_command_any(cmd, ass_or_exp, is_bad, cmd_type) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}