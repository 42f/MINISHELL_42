/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp_av_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:15:57 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:16:00 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_new_ac(char **av, size_t size)
{
	size_t	i;
	size_t	new_ac;

	i = 0;
	new_ac = 0;
	while (i < size)
	{
		if (av[i] != NULL && ft_strlen(av[i]) != 0)
			new_ac++;
		i++;
	}
	return (new_ac);
}

void	fill_envp(t_cmd *cmd, int index_cmd)
{
	size_t	i;
	size_t	i_envp;

	i_envp = 0;
	i = 0;
	while (i < (size_t)index_cmd)
	{
		if (cmd->av[i] != NULL && ft_strlen(cmd->av[i]) != 0)
		{
			cmd->envp[i_envp] = ft_strdup(cmd->av[i]);
			ft_strdel(&cmd->av[i]);
			i_envp++;
		}
		else
			free(cmd->av[i]);
		i++;
	}
}

int		clean_cmd_av(t_cmd *cmd, t_clean_cmd *clean_cmd, bool no_cmd,
					int index_cmd)
{
	size_t	i;

	if (no_cmd == false)
		clean_cmd->ac = cmd->ac - index_cmd;
	else
	{
		i = 0;
		while (i < (size_t)cmd->ac)
		{
			free(cmd->av[i]);
			i++;
		}
		free(cmd->av);
		cmd->av = NULL;
		cmd->ac = 0;
		return (NO_COMMAND);
	}
	return (SUCCESS);
}

int		fill_clean_cmd(int index_cmd, t_cmd *cmd, t_clean_cmd *clean_cmd)
{
	size_t	i_clean;
	size_t	i;

	clean_cmd->av = (char **)malloc(sizeof(char *) * clean_cmd->ac);
	if (clean_cmd->av == NULL)
		return (FAILURE);
	i = index_cmd;
	clean_cmd->index_cmd = index_cmd;
	i_clean = 0;
	while (i_clean < clean_cmd->ac)
	{
		if (cmd->av[i] != NULL)
		{
			clean_cmd->av[i_clean] = ft_strdup(cmd->av[i]);
			free(cmd->av[i]);
		}
		if (cmd->av[i] == NULL)
			clean_cmd->av[i_clean] = NULL;
		i_clean++;
		i++;
	}
	free(cmd->av);
	return (SUCCESS);
}

int		fill_cmd(t_cmd *cmd, t_clean_cmd *clean_cmd)
{
	size_t	i;

	cmd->ac = clean_cmd->ac;
	cmd->av = (char **)malloc(sizeof(char *) * cmd->ac);
	if (cmd->av == NULL)
		return (FAILURE);
	i = 0;
	while (i < (size_t)cmd->ac)
	{
		if (clean_cmd->av[i] != NULL)
			cmd->av[i] = ft_strdup(clean_cmd->av[i]);
		if (clean_cmd->av[i] == NULL)
			cmd->av[i] = NULL;
		i++;
	}
	return (SUCCESS);
}
