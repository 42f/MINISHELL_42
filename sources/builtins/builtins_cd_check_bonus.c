#include "minishell_bonus.h"

int			process_error(t_vector *vct_home, char *dir, t_vector *vct_old_pwd)
{
	if (vct_home == NULL && dir == NULL)
	{
		print_set_errno(0, "HOME not set", STR_CD, NULL);
		return (CD_FAIL);
	}
	if (ft_strequ(STR_MINUS, dir) == TRUE && vct_getstr(vct_old_pwd) == NULL)
	{
		print_set_errno(0, "OLDPWD not set", STR_CD, NULL);
		return (CD_FAIL);
	}
	if (vct_getlen(vct_old_pwd) == 0 && ft_strequ(dir, STR_MINUS) == TRUE)
	{
		print_set_errno(0, "OLDPWD has no value", STR_CD, NULL);
		return (CD_FAIL);
	}
	if (dir == NULL && vct_getlen(vct_home) == 0)
	{
		print_set_errno(0, "HOME has no value", STR_CD, NULL);
		return (CD_FAIL);
	}
	return (SUCCESS);
}

int			check_cd_arg(int ac)
{
	if (ac > 2)
	{
		print_set_errno(0, "too many arguments", STR_CD, NULL);
		return (CD_FAIL);
	}
	return (CD_CONTINUE);
}

static int	check_directory(char *directory)
{
	size_t i;
	size_t count_dot;

	i = 2;
	count_dot = 2;
	if (ft_strlen(directory) > 2 && directory[0] == DOT && directory[1] == DOT)
	{
		while (directory[i] != '\0')
		{
			if (count_dot == 2 && directory[i] == DOT)
				return (FAILURE);
			if (count_dot == 2 && directory[i] != DOT)
				count_dot = 0;
			if (directory[i] == DOT)
				count_dot++;
			i++;
		}
	}
	return (SUCCESS);
}

int			first_check(char *directory)
{
	DIR *dir;
	int ret_directory;

	dir = opendir(directory);
	ret_directory = check_directory(directory);
	if (dir == NULL && errno == PERMISSION_DENIED && ret_directory == SUCCESS)
		return (CD_CONTINUE);
	if (dir == NULL)
	{
		if (ret_directory == FAILURE && errno == PERMISSION_DENIED)
			print_set_errno(2, strerror(2), STR_CD, directory);
		else
			print_set_errno(errno, strerror(errno), STR_CD, directory);
		return (CD_FAIL);
	}
	if (closedir(dir) == FAILURE)
	{
		print_set_errno(errno, strerror(errno), "closedir", NULL);
		exit(FAILURE);
	}
	return (CD_CONTINUE);
}

int			handle_permission_denied(char **dir, char *dir_denied)
{
	char		*pwd;
	char		*buff;
	t_vector	*new_dir;

	buff = (char *)malloc(sizeof(char) * (PATH_MAX + 1));
	if (buff == NULL)
	{
		print_set_errno(0, ERR_MALLOC, NULL, NULL);
		exit (FAILURE);
	}
	pwd = getcwd(buff, PATH_MAX);
	if (pwd == NULL)
	{
		print_set_errno(errno, ERR_MALLOC, NULL, NULL);
		free(buff);
		exit(FAILURE);
	}
	new_dir = vct_new();
	transform_new_dir(new_dir, pwd, dir_denied);
	if (vct_getlen(new_dir) == 0)
		*dir = ft_strdup(STR_ROOT);
	else
		*dir = ft_strdup(vct_getstr(new_dir));
	vct_del(&new_dir);
	free(buff);
	return (SUCCESS);
}
