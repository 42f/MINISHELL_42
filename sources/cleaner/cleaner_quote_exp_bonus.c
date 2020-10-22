#include "minishell_bonus.h"

char	*between_nothing(char *str)
{
	char	*good_str;
	t_vector	*vct_good;
	bool		is_exp;
	size_t		i;

	good_str = NULL;
	vct_good = vct_new();
	is_exp = false;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != EXP || (str[i] == EXP && i == ft_strlen(str) - 1))
			vct_add(vct_good, str[i]);
		else if (str[i] == EXP)
		{
			i = handle_exp(i, vct_good, str);
			continue ;
		}
		i++;
	}
	good_str = ft_strdup(vct_getstr(vct_good));
	vct_del(&vct_good);
	return (good_str);
}

char	*between_simple(char *str)
{
	char	*good_str;
	size_t	i;

	ft_printf("################## BETWEEN_SIMPLE #################\n");//DEBUG
	ft_printf("STR = %s\n", str);//DEBUG
	good_str = NULL;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == C_SIMPLE_QUOTE)
			handle_simple(str, i);
		i++;
	}
	return (good_str);
}

char	*between_double(char *str)
{
	char	*good_str;

	ft_printf("BETWEEN_DOUBLE\n");//DEBUG
	good_str = NULL;
	return (good_str);
}

char	*between_both(char *str)
{
	char	*good_str;

	ft_printf("BETWEEN_BOTH\n");//DEBUG
	good_str = NULL;
	return (good_str);
}
