/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_no_word_parse.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:19:04 by lfallet           #+#    #+#             */
/*   Updated: 2020/12/03 16:19:05 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_backslash(t_vector *input, t_vector *word, bool is_quoting)
{
	const char c = vct_getfirstchar(input);
	const char c_next = vct_getcharat(input, 1);

	if (c != C_BACKSLASH)
		return (false);
	if ((is_quoting == true && (c_next == C_EXP || c_next == C_BACKSLASH ||
								c_next == C_QUOTE)) ||
		is_quoting == false)
	{
		if (is_quoting == true)
		{
			safe_vct_add(word, c);
			vct_pop(input);
		}
		return (true);
	}
	return (false);
}

int		backslash(char c, t_vector *word, t_vector *input, char next_c)
{
	safe_vct_add(word, c);
	vct_pop(input);
	if (next_c == '\0')
	{
		print_set_errno(0, ERR_NEWLINE, NULL, NULL);
		ms_setenv_int(get_env_list(GET), S_QUESTION_MARK, 2, F_OVERWRITE);
		return (FAILURE);
	}
	safe_vct_add(word, next_c);
	vct_pop(input);
	return (TRUE);
}

int		handle_quote(char c, bool quote_state, bool dquote_state,
						t_vector *input)
{
	if (quote_state == false && dquote_state == false && (c == C_SPACE ||
			c == C_TAB))
	{
		vct_pop(input);
		return (SUCCESS);
	}
	if (dquote_state == false && quote_state == false && is_end(input) == true)
		return (SUCCESS);
	return (FAILURE);
}

int		backsl_quote(bool quote_state, bool dquote_state, t_vector *input,
						t_vector *word)
{
	int ret_parse;

	ret_parse = 0;
	if (quote_state == false)
	{
		if (vct_getfirstchar(input) == C_BACKSLASH)
			ret_parse = parse_backslash(input, word, dquote_state);
		if (ret_parse == false)
			if (is_newline_error(dquote_state, quote_state, input) == FAILURE)
				return (FAILURE);
	}
	return (SUCCESS);
}
