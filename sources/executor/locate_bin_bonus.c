#include "minishell_bonus.h"

static char	**get_all_path_directories(void)
{
	char		*path_env;
	char		**ret_all_paths;

	ret_all_paths = NULL;
	path_env = vct_getstr(get_env_value_vct(get_env_list(GET), "PATH"));
	if (path_env == NOT_FOUND)
		return (NULL);
	else
	{
		ret_all_paths = ft_split(path_env, ':');
		if (ret_all_paths == NULL)
			exit_routine_le(ERR_MALLOC);
	}
	return (ret_all_paths);
}

static int	stat_path(const char *path_to_stat)
{
	struct stat	statbuf;
	int			ret;

	errno = 0;
	ret = stat(path_to_stat, &statbuf);
	if (S_ISDIR(statbuf.st_mode) != 0)
		errno = EISDIR;
	return ((ret != FAILURE && errno == 0) ? SUCCESS : FAILURE);
}

static void	concat_path(t_vector **vct, char *dir_option, char *bin_name)
{
	if (*vct == NULL)
		exit_routine_le(ERR_MALLOC);
	vct_addstr(*vct, (char *)dir_option);
	vct_addstr(*vct, "/");
	vct_addstr(*vct, (char *)bin_name);
}

static char	*check_dir_option(const char *bin_name, const char *dir_option)
{
	t_vector	*full_path_vct;
	char		*ret_full_path;
	int			ret;

	errno = 0;
	ret_full_path = NOT_FOUND;
	full_path_vct = vct_new();
	concat_path(&full_path_vct, (char *)dir_option, (char *)bin_name);
	if ((ret = stat_path(vct_getstr(full_path_vct))) == SUCCESS)
		ret_full_path = ft_strdup(vct_getstr(full_path_vct));
	else if (ret == FAILURE && errno != ENOENT && errno != ENAMETOOLONG)
		print_set_errno(errno, NULL, ret_full_path, NULL);
	vct_del(&full_path_vct);
	return (ret_full_path);
}

char		*locate_binary_file(const char *bin_name)
{
	char		**dir_options;
	char		*ret_full_path;
	int			i;

	ret_full_path = NOT_FOUND;
	if (is_path(bin_name) == TRUE)
	{
		if (stat_path(bin_name) == SUCCESS)
			ret_full_path = ft_strdup(bin_name);
		else
			print_set_errno(errno, NULL, bin_name, NULL);
	}
	else
	{
		dir_options = get_all_path_directories();
		if (dir_options != NULL && dir_options[0] != NULL)
		{
			i = 0;
			while (dir_options[i] != NULL && ret_full_path == NOT_FOUND)
				ret_full_path = check_dir_option(bin_name, dir_options[i++]);
			free_char_arr(dir_options);
		}
	}
	return (ret_full_path);
}
