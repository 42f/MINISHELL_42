#include "minishell_bonus.h"

void	put_env(char *env_name, t_vector *env_value)
{
	ft_putstr_fd(env_name, STDERR_FILENO);
	if (env_value != NULL)
		ft_printf("=%s", vct_getstr(env_value));
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	print_env(char *env_name)
{
	char	**envp;
	int		i;
(void)env_name;
	i = 0;
	envp = get_env_data(GET)->envp;
	while (envp[i] != NULL)
	{
		ft_putstr_fd(envp[i], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		i++;
	}
}

/*
void	print_env(char *env_name)
{
	t_vector	*env_value;
	t_list		*cursor;

	cursor = get_env_data(GET)->env_lst;
	if (env_name == ALL)
	{
		while (cursor != NULL)
		{
			if (((t_env *)cursor->content)->export_flag == TRUE)
			{
				env_name = ((t_env *)cursor->content)->env_name;
				env_value = ((t_env *)cursor->content)->env_value;
				put_env(env_name, env_value);
			}
			cursor = cursor->next;
		}
	}
	else //for debug, will remove once 'env | grep ...' works
	{
		cursor = get_env_node(env_name);
		if (cursor != NOT_FOUND)
		{
			env_value = ((t_env*)cursor->content)->env_value;
			put_env(env_name, env_value);
		}
	}
}
*/
