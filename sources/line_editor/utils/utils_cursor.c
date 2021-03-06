/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cursor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:20:06 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:20:07 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	move_at_col_x(int target_col)
{
	int		i;
	t_le	*le;

	le = get_struct(GET);
	if (le->termcap[MOVE_AT_COL_X] != NULL)
		ms_tputs(tparm(le->termcap[MOVE_AT_COL_X], target_col), 1, ms_putchar);
	else
	{
		ms_tputs(le->termcap[RETURN_CARRIAGE], 1, ms_putchar);
		i = 0;
		while (i < target_col)
		{
			ms_tputs(le->termcap[ONE_COL_RIGHT], 1, ms_putchar);
			i++;
		}
	}
}

int		move_cursor_left(void)
{
	t_le	*le;

	le = get_struct(GET);
	if (le->vct_index <= 0)
		return (FAILURE);
	if (le->cx == 0 && le->cy > 0)
	{
		ms_tputs(le->termcap[ONE_ROW_UP], 1, ms_putchar);
		move_at_col_x(le->scols);
		le->cx = le->scols - 1;
		le->cy--;
	}
	else
	{
		ms_tputs(le->termcap[ONE_COL_LEFT], 1, ms_putchar);
		le->cx--;
	}
	le->vct_index--;
	return (SUCCESS);
}

int		move_cursor_right(void)
{
	t_le	*le;

	le = get_struct(GET);
	if (le->vct_index >= (int)vct_getlen(le->cmd_line))
		return (FAILURE);
	if (le->cx >= le->scols - 1)
	{
		move_at_col_x(0);
		ms_tputs(le->termcap[ONE_ROW_DOWN], 1, ms_putchar);
		le->cx = 0;
		le->cy++;
	}
	else
	{
		ms_tputs(le->termcap[ONE_COL_RIGHT], 1, ms_putchar);
		le->cx++;
	}
	le->vct_index++;
	return (SUCCESS);
}

void	move_cursor_at_index(int index_to)
{
	int		index_delta;
	int		offset;
	t_le	*le;

	le = get_struct(GET);
	offset = (le->cy == 0) ? le->prompt_len : 0;
	index_delta = index_to - le->vct_index;
	if (index_delta < 0)
		return ;
	le->cy += (index_delta + offset) / le->scols;
	le->cx = (index_delta + offset) % le->scols;
	le->vct_index = index_to;
}

void	move_previous_line_head(void)
{
	int		offset;
	t_le	*le;

	le = get_struct(GET);
	if (le->vct_index == 0)
		return ;
	if (le->cy > 0)
	{
		ms_tputs(le->termcap[ONE_ROW_UP], 1, ms_putchar);
		le->cy--;
	}
	offset = (le->cy == 0) ? le->prompt_len : 0;
	move_at_col_x(offset);
	le->cx = offset;
	if (le->cy == 0)
		le->vct_index = 0;
	else
		le->vct_index = (le->scols * (le->cy - 1)) + le->scols - le->prompt_len;
}
