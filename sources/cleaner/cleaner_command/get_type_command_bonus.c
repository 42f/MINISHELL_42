#include "minishell_bonus.h"

bool	verif_assign_cmd(char *str)
{
	t_vector	*vct;
	size_t		id_equal;
	ssize_t		nb_assign;
	t_vector	*vct_cpy;

	vct = vct_new();
	vct_cpy = vct_new();
	vct_addstr(vct, str);
	vct_addstr(vct_cpy, vct_getstr(vct));
	nb_assign = vct_nbchar(vct_cpy, S_ASSIGN);
	if (nb_assign > 1)
	{
		vct_del(&vct);
		vct_del(&vct_cpy);
		return (true);
	}
	id_equal = vct_clen(vct, ASSIGN);
	vct_cutfrom(vct, vct_getlen(vct) - id_equal);
	if (vct_chr(vct, C_PATH) != FAILURE)
	{
		vct_del(&vct);
		vct_del(&vct_cpy);
		return (true);
	}
	if (is_wrong_ass(vct) == false)
	{
		vct_del(&vct);
		vct_del(&vct_cpy);
		return (true);
	}
	if (vct_getcharat(vct, vct_getlen(vct) - 1) == C_PLUS)
	{
		vct_del(&vct);
		vct_del(&vct_cpy);
		return (false);
	}
	if (vct_chr(vct, C_PLUS) != FAILURE || vct_chr(vct, C_MINUS) != FAILURE)
	{
		vct_del(&vct);
		vct_del(&vct_cpy);
		return (true);
	}
	vct_del(&vct);
		vct_del(&vct_cpy);
	return (false);
}

static bool	verif_exp_cmd(char *str)
{
	t_vector	*vct;

	vct = vct_new();
	vct_addstr(vct, str);
	if (vct_getfirstchar(vct) == C_EXPORT)
		vct_pop(vct);
	if (is_wrong_ass(vct) == false)
	{
		vct_del(&vct);
		return (true);
	}
	vct_del(&vct);
	return (false);
}

static int	process_get_cmd(size_t i_assign, size_t i_exp, size_t i, t_cmd *cmd)
{
	if (i_assign < (size_t)cmd->count_assign && cmd->count_assign != 0
			&& i == (size_t)cmd->tab_assign[i_assign])
	{
		if (ft_strchr(cmd->av[i], ASSIGN) == NULL)
			return (TRUE_CMD);
		if (verif_assign_cmd(cmd->av[i]) == false)
			return (TRUE_ASSIGN);
		return (FALSE_ASSIGN);
	}
	else if (i_exp < (size_t)cmd->count_exp && cmd->count_exp != 0
				&& i == (size_t)cmd->tab_exp[i_exp])
	{
		if (ft_strchr(cmd->av[i], EXP) == NULL)
			return (TRUE_CMD);
		if (verif_exp_cmd(cmd->av[i]) == false)
			return (TRUE_EXP);
		return (FALSE_EXP);
	}
	return (TRUE_CMD);
}

int			get_cmd(t_cmd *cmd)
{
	size_t	i;
	size_t	i_assign;
	size_t	i_exp;
	int		ret;

	i = 0;
	i_assign = 0;
	i_exp = 0;
	while (i < (size_t)cmd->ac)
	{
//		ft_printf("cmd->av[%d] = %s\n", i, cmd->av[i]);//DEBUG
//		ft_printf("cmd->ac = %d\n", cmd->ac);//DEBUG`
		if (ft_strlen(cmd->av[i]) == 0 && i + 1 != (size_t)cmd->ac)
		{
			i++;
			continue ;
		}
		ret = process_get_cmd(i_assign, i_exp, i, cmd);
		if (ret == FALSE_ASSIGN || ret == FALSE_EXP || ret == TRUE_CMD)
			return (i);
		if (ret == TRUE_ASSIGN)
			i_assign++;
		else if (ret == TRUE_EXP)
			i_exp++;
		i++;
	}
	return (FAILURE);
}
