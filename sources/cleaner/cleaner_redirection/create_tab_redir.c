/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tab_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:17:18 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:17:19 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static enum e_state_redir	in_out(char *str, enum e_token_type type)
{
	if ((type != E_LESS_THAN && type != E_GREATER_THAN &&
			type != E_DOUBLE_GREATER))
		return (E_IN_OUT);
	if ((ft_strequ(str, LESS_THAN) == TRUE ||
			ft_strequ(str, GREATER_THAN) == TRUE ||
			ft_strequ(str, DOUBLE_GREATER) == TRUE) && type != E_EXP)
		return (E_IN_REDIR);
	return (E_IN_OUT);
}

static enum e_state_redir	in_file(char *str, enum e_token_type type)
{
	if ((type != E_LESS_THAN && type != E_GREATER_THAN &&
			type != E_DOUBLE_GREATER))
		return (E_IN_OUT);
	if ((ft_strequ(str, LESS_THAN) == TRUE ||
			ft_strequ(str, GREATER_THAN) == TRUE ||
			ft_strequ(str, DOUBLE_GREATER) == TRUE) && type != E_EXP)
		return (E_IN_REDIR);
	return (E_IN_OUT);
}

static enum e_state_redir	in_redir(char *str, enum e_token_type type)
{
	(void)str;
	(void)type;
	return (E_IN_FILE);
}

int							create_tab_redir(t_cmd *cmd, t_clean_cmd *cl)
{
	static t_state_redir	function_state[] = {in_redir, in_file, in_out};
	enum e_state_redir		state;
	size_t					i;

	state = E_IN_OUT;
	i = 0;
	cl->tmp_tab_redir = (char **)malloc(sizeof(char *) * cmd->ac);
	if (cl->tmp_tab_redir == NULL)
		return (FAILURE);
	while (i < (size_t)cmd->ac)
	{
		cl->tmp_tab_redir[i] = NULL;
		state = function_state[state](cmd->av[i], cmd->type[cl->index_cmd]);
		if (state == E_IN_REDIR || state == E_IN_FILE)
		{
			if (cmd->av[i] != NULL)
				cl->tmp_tab_redir[i] = ft_strdup(cmd->av[i]);
			free(cmd->av[i]);
			cmd->av[i] = NULL;
		}
		cl->index_cmd++;
		i++;
	}
	return (SUCCESS);
}
