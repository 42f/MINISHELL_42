#include "minishell_bonus.h"

static int	handle_pwd(int flag)
{
	char *pwd;
	char *buff;

	buff = (char *)malloc(sizeof(char) * (PATH_MAX + 1));
	if (buff == NULL)
	{
		free(buff);
		print_set_errno(errno, ERR_MALLOC, NULL, NULL);
		exit(FAILURE);
	}
	pwd = getcwd(buff, PATH_MAX);
	if (pwd == NULL)
	{
		ft_putendl_fd("chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", STDERR_FILENO);
		free(buff);
		return (FAILURE);
	}
	if (flag == PWD)
		ms_setenv(get_env_list(GET), ENV_PWD, pwd, F_EXPORT | F_OVERWRITE);
	else if (flag == OLD_PWD)
		ms_setenv(get_env_list(GET), ENV_OLD_PWD, pwd, F_EXPORT | F_OVERWRITE);
	free(buff);
	return (SUCCESS);
}

static int	process_chdir(t_vector *vct_home, char *dir)
{
	char	*real_dir;
	char	*dir_denied;
	int		ret_chdir;

	real_dir = ft_strdup(dir == NULL && vct_getlen(vct_home) != 0 ?
					vct_getstr(vct_home) : dir);
	ret_chdir = chdir(real_dir);
	if (ret_chdir == FAILURE && ft_strnequ(real_dir, DOTDOT, 2) == TRUE)
	{
		dir_denied = ft_strdup(real_dir);
		free(real_dir);
		handle_permission_denied(&real_dir, dir_denied);
		free(dir_denied);
		ret_chdir = chdir(real_dir);
	}
	if (ret_chdir == FAILURE)
		print_set_errno(errno, strerror(errno), STR_CD, real_dir);
	free(real_dir);
	if (ret_chdir == SUCCESS)
		ret_chdir = handle_pwd(PWD);
	return (ret_chdir);
}

static int	hub_process_chdir(char *dir, t_vector *vct_home)
{
	t_vector	*dir_old_pwd;
	char		*str_old_pwd;
	int			ret_chdir;
	int			ret_pwd;

	str_old_pwd = NULL;
	if (ft_strequ(dir, STR_MINUS) == TRUE)
	{
		dir_old_pwd = get_env_value_vct(get_env_list(GET), "OLDPWD");
		str_old_pwd = vct_getstr(dir_old_pwd);
		ft_printf("%s\n", str_old_pwd);
		ret_chdir = process_chdir(vct_home, str_old_pwd);
		return (ret_chdir == FAILURE ? CD_FAIL : SUCCESS);
	}
	ret_pwd = handle_pwd(OLD_PWD);
	if (ret_pwd != SUCCESS)
		return (ret_pwd);
	ret_chdir = process_chdir(vct_home, dir);
	return (ret_chdir);
}

static int	process_cd(char *dir)
{
	t_vector *vct_home;
	t_vector *vct_old_pwd;

	vct_home = get_env_value_vct(get_env_list(GET), ENV_HOME);
	vct_old_pwd = get_env_value_vct(get_env_list(GET), ENV_OLD_PWD);
	if (process_error(vct_home, dir, vct_old_pwd) == CD_FAIL)
		return (CD_FAIL);
	return (hub_process_chdir(dir, vct_home) == FAILURE ? CD_FAIL : SUCCESS);
}

int			cd_builtin(int ac, char **av, char **envp)
{
	int			ret_check;

	(void)envp;
	if (check_cd_arg(ac) == CD_FAIL)
		return (CD_FAIL);
	if (ac != 1 && ft_strlen(av[1]) != 0 &&
			ft_strequ(av[1], STR_MINUS) == FALSE)
	{
		ret_check = first_check(av[1]);
		if (ret_check != CD_CONTINUE)
			return (ret_check == CD_FAIL ? 2 : FAILURE);
	}
	if (ac != 1 && ft_strlen(av[1]) == 0)
		return (SUCCESS);
	return (process_cd(av[1]));
}
