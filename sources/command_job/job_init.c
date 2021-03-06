/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:18:43 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:18:44 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		add_job_to_list(t_job *job, t_list **jobs, t_list *token_list,
							t_list **head)
{
	t_list	*node_job;

	node_job = ft_lstnew(job);
	if (node_job == NULL)
	{
		ft_lstdelone(node_job, NULL);
		exit_routine(EXIT_MALLOC);
	}
	ft_lstadd_back(jobs, node_job);
	*head = token_list;
	return (SUCCESS);
}

t_job	*init_job(void)
{
	t_job	*job;

	job = (t_job *)malloc(sizeof(t_job));
	if (job == NULL)
		exit_routine(EXIT_MALLOC);
	job->nb_cmd = 0;
	job->last_pid = 0;
	job->cmd_lst = NULL;
	return (job);
}
