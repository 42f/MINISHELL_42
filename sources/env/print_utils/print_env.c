/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:15:44 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:15:45 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_env(char *env_name, t_vector *env_value)
{
	char	*env_value_str;

	ft_putstr_fd(env_name, STDOUT_FILENO);
	env_value_str = vct_getstr(env_value);
	if (env_value_str != NULL)
		ft_printf("=%s", env_value_str);
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	print_env(t_list *env_lst)
{
	char	**envp;
	int		i;

	i = 0;
	envp = get_envp(env_lst);
	while (envp[i] != NULL)
	{
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	free_char_arr(envp);
}
