/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:14:42 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:14:43 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_old_pwd(char *dir, char *pwd, int flag)
{
	if (flag == PWD_HOME)
	{
		ms_setenv(get_env_list(GET), ENV_PWD, dir == NULL ?
					get_env_value_str(ENV_HOME) : dir, F_EXPORT | F_OVERWRITE);
		ms_setenv(get_env_list(GET), ENV_OLD_PWD,
					get_env_value_str(ENV_PWD), F_EXPORT | F_OVERWRITE);
	}
	else if (flag == PWD_OLDPWD)
	{
		if (ft_strequ(pwd, get_env_value_str(ENV_PWD)) == FALSE)
		{
			ms_setenv(get_env_list(GET), ENV_OLD_PWD, pwd, F_EXPORT |
						F_OVERWRITE);
			ms_setenv(get_env_list(GET), ENV_PWD,
						get_env_value_str(ENV_OLD_PWD), F_EXPORT | F_OVERWRITE);
		}
	}
}
