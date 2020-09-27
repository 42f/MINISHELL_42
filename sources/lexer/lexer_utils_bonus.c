#include "minishell_bonus.h"

void		exit_routine_lexer(t_vector *word, t_vector *vct, t_vector *tmp,
								t_token *token)
{
	if (word != NULL)
		vct_del(&word);
	if (vct != NULL)
		vct_del(&vct);
	if (tmp != NULL)
		vct_del(&tmp);
	if (token != NULL)
	{
		free(token->data);
		token->type = 0;
	}
}

static int	handle_assign(t_vector *vct, t_vector *input, t_list **token_list)
{
	t_vector	*tmp;
	int			ret_extract;
	size_t		len_tmp;

	tmp = vct_cdup(input, ' ');
	len_tmp = vct_getlen(tmp);
	vct = vct_join(vct, tmp);
	ret_extract = extract_token(token_list, vct_getstr(vct), E_ASSIGN);
	if (ret_extract == FAILURE)
	{
		exit_routine_lexer(NULL, vct, tmp, NULL);
		free_list(token_list);
		return (FALSE);
	}
	vct_popfrom(input, len_tmp);
	vct_del(&tmp);
	vct_del(&vct);
	return (TRUE);
}

static int	no_assign(t_list **token_list, t_vector *vct, int ret)
{
	int	ret_extract;

	ret_extract = extract_token(token_list, vct_getstr(vct),
					ret == N_SIMPLE_QUOTE ? E_SIMPLE_QUOTE : E_QUOTE);
	if (ret_extract == FAILURE)
	{
		exit_routine_lexer(NULL, vct, NULL, NULL);
		free_list(token_list);
		return (FALSE);
	}
	return (TRUE);
}

int			handle_quote(t_vector *input, t_list **token_list, int ret)
{
	t_vector	*vct;
	size_t		len;
	int			ret_extract;

	vct = vct_cdup(input, ret == N_SIMPLE_QUOTE ? C_SIMPLE_QUOTE : C_QUOTE);
	if (vct_equ(input, vct) == TRUE)
	{
		vct_del(&vct);
		free_list(token_list);
		return (FALSE);
	}
	vct_add(vct, ret == N_SIMPLE_QUOTE ? C_SIMPLE_QUOTE : C_QUOTE);
	vct_addcharat(vct, FIRST_CHAR, ret == N_SIMPLE_QUOTE
			? C_SIMPLE_QUOTE : C_QUOTE);
	len = vct_getlen(vct);
	vct_popfrom(input, len - 1);
	if (vct_getcharat(input, FIRST_CHAR) == '=')
		ret_extract = handle_assign(vct, input, token_list);
	else
		ret_extract = no_assign(token_list, vct, ret);
	vct_del(&vct);
	return (ret_extract);
}