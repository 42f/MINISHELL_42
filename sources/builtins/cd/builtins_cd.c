/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:14:18 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:14:20 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			handle_pwd(char *dir)
{
	char *pwd;

	if ((pwd = getcwd(NULL, PATH_MAX)) == NULL)
	{
		if (dir && ft_strlen(dir) != 0 && dir[0] != DOT && dir[0] != C_PATH)
		{
			ft_dprintf(2, "%s %s: %s\n", CD_BUILT, dir, strerror(errno));
			return (FAILURE);
		}
		else if (dir == NULL || (ft_strlen(dir) != 0 && dir[0] == C_PATH))
		{
			set_old_pwd(dir, pwd, PWD_HOME);
			return (SUCCESS);
		}
		return (errno == 2 ? 2 : FAILURE);
	}
	set_old_pwd(dir, pwd, PWD_OLDPWD);
	free(pwd);
	return (SUCCESS);
}

static int	process_chdir(t_vector *vct_home, char *dir, char *old_dir,
							char *dir_old_pwd)
{
	char	*real_dir;
	int		ret_chdir;

	real_dir = !dir && vct_getlen(vct_home) ? vct_getstr(vct_home) : dir;
	if ((ret_chdir = chdir(real_dir)) == SUCCESS)
	{
		ret_chdir = handle_pwd(dir);
		if (old_dir != NULL && ft_strequ(old_dir, STR_MINUS) == TRUE)
			ft_printf("%s\n", dir_old_pwd);
	}
	if (ret_chdir == FAILURE && ft_strnequ(real_dir, DOTDOT, 2) == TRUE)
	{
		real_dir = handle_permission_denied(real_dir);
		ret_chdir = chdir(real_dir);
		free(real_dir);
	}
	if (ret_chdir == FAILURE)
	{
		ft_dprintf(STDERR_FILENO, "Minishell: cd: %s: %s\n",
					dir, strerror(errno));
	}
	free(dir_old_pwd);
	return (ret_chdir);
}

static int	hub_process_chdir(char *dir, t_vector *vct_home)
{
	int			ret_chdir;
	int			ret_pwd;
	t_vector	*pwd;
	char		*old_dir;

	if (ft_strequ(dir, STR_MINUS) == TRUE)
	{
		old_dir = (dir == NULL) ? NULL : ft_strdup(dir);
		pwd = get_env_value_vct(get_env_list(GET), ENV_OLD_PWD);
		ret_chdir = process_chdir(vct_home, vct_getstr(pwd),
									old_dir, vct_strdup(pwd));
		free(old_dir);
		return (ret_chdir == FAILURE ? CD_FAIL : SUCCESS);
	}
	ret_pwd = handle_pwd(dir);
	if (ret_pwd != SUCCESS)
		return (ret_pwd == 3 ? 1 : ret_pwd);
	ret_chdir = process_chdir(vct_home, dir, NULL, NULL);
	return (ret_chdir);
}

static int	process_cd(char *dir)
{
	t_vector	*vct_home;
	t_vector	*vct_old_pwd;
	int			ret_process_chdir;

	vct_home = get_env_value_vct(get_env_list(GET), ENV_HOME);
	vct_old_pwd = get_env_value_vct(get_env_list(GET), ENV_OLD_PWD);
	if (dir == NULL && vct_home != NULL)
		dir = vct_getstr(vct_home);
	if (process_error(vct_home, dir, vct_old_pwd) == CD_FAIL)
		return (CD_FAIL);
	ret_process_chdir = hub_process_chdir(dir, vct_home);
	if (ret_process_chdir == FAILURE)
		ret_process_chdir = CD_FAIL;
	return (ret_process_chdir);
}

int			cd_builtin(int ac, char **av, __attribute__((unused)) char **envp)
{
	int		ret;
	char	*pwd;

	if (av == NULL || check_cd_arg(ac) == CD_FAIL)
		return (CD_FAIL);
	if (ac > 1 && ft_strlen(av[1]) != 0 && ft_strequ(av[1], STR_MINUS) == FALSE)
	{
		if ((ret = first_check(av[1])) != CD_CONTINUE)
			return (ret == CD_FAIL ? 1 : 2);
	}
	if (ac != 1 && ft_strlen(av[1]) == 0)
		return (SUCCESS);
	pwd = get_env_value_str(ENV_PWD);
	pwd = pwd != NULL ? ft_strdup(pwd) : getcwd(NULL, PATH_MAX);
	ret = process_cd(av[1]);
	if (ret == 2 && (ac > 1 && ft_strlen(av[1]) != 0 && av[1][0] != C_PATH))
	{
		ret = 1;
		print_set_errno(0, ERR_NO_FILE, STR_CD, av[1]);
	}
	if (ret == SUCCESS)
		ms_setenv(get_env_list(GET), ENV_OLD_PWD, pwd, F_EXPORT | F_OVERWRITE);
	free(pwd);
	return (ret);
}
