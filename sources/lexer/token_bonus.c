#include "minishell_bonus.h"

ssize_t	get_double_token(t_vector *input)
{
	if (vct_chrnstr(input, DOUBLE_GREATER) == FALSE)
		return (E_DOUBLE_GREATER);
	else if (vct_chrnstr(input, OR) == FALSE)
		return (E_OR);
	else if (vct_chrnstr(input, AND) == FALSE)
		return (E_AND);
	return (NO_TYPE);
}

size_t	get_token(char c)
{
	const char	*grammar = GRAMMAR;
	size_t		i;

	i = 0;
	while (i < NB_GRAMMAR)
	{
		if (c == grammar[i])
			return (i);
		i++;
	}
	return (E_WORD);
}