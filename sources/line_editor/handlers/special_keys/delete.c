/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:19:01 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:19:02 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_selection(long key)
{
	t_le	*le;
	int		i;

	le = get_struct(GET);
	if (le->select_min == -1)
	{
		if (key == K_DEL_BACKWARD && le->vct_index == 0)
			return ;
		if (key == K_DEL_BACKWARD)
			move_cursor_left();
		vct_popcharat(le->cmd_line, le->vct_index);
	}
	else
	{
		i = le->select_max;
		while (i >= 0 && i >= le->select_min)
			vct_popcharat(le->cmd_line, i--);
		unselect_all();
	}
}
