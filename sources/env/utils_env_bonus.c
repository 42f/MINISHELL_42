#include "minishell_bonus.h"

void	free_env_list(t_list *env_lst)
{
	if (env_lst != NULL)
	{
		ft_lstclear(&env_lst, del_env_elem);
	}
}

void	free_btree_node(t_btree *node)
{
	free((void *)node);
}

void	ms_putenv(t_list *env_lst, const char *env)
{
	store_env(env_lst, env, F_NOFLAG);
}

void	export_env(t_list *env_lst, const char *env)
{
	store_env(env_lst, env, F_EXPORT);
}
