#include "minishell_bonus.h"

int			safe_vct_cpy(t_vector *dest, t_vector *src)
{
	int			ret;

	if (dest == NULL || src == NULL || dest->str == NULL || src->str == NULL)
		return (FAILURE);
	ret = vct_cpy(dest, src);
	if (ret == FAILURE)
		exit_routine(EXIT_VCT);
	return (ret);
}

int			safe_vct_add(t_vector *vct, char c)
{
	int			ret;

	ret = vct_add(vct, c);
	if (ret == FAILURE)
		exit_routine(EXIT_VCT);
	return (ret);
}

int			safe_vct_addstr(t_vector *vct, char *str)
{
	int			ret;

	ret = vct_addstr(vct, str);
	if (ret == FAILURE)
		exit_routine(EXIT_VCT);
	return (ret);
}

t_vector	*safe_vct_new(void)
{
	t_vector	*new_vct;

	new_vct = vct_new();
	if (new_vct == NULL)
		exit_routine(EXIT_VCT);
	return (new_vct);
}

int			safe_vct_addcharat(t_vector *vct, size_t index, char c)
{
	int			ret;

	if (vct == NULL || index > vct_getlen(vct) || c == '\0')
		return (FAILURE);
	ret = vct_addcharat(vct, index, c);
	if (ret == FAILURE)
		exit_routine(EXIT_VCT);
	return (ret);
}