/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:16:10 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:16:11 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_environ(char *environ)
{
	char	*env_name;
	char	*env_value;
	int		overwrite;
	int		ret;

	if (ft_strnstr(environ, "+=", ft_strlen(environ)) != NOT_FOUND)
		return (true);
	parse_env(environ, &env_name, &env_value, &overwrite);
	ret = is_valid_export_identifier(env_name);
	free(env_name);
	if (env_value != NULL)
		free(env_value);
	if (ret == false)
		return (true);
	else if (ft_isalpha(*environ) == false)
		return (true);
	return (false);
}
