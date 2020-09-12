#include "line_editor_bonus.h"

void	update_selection(t_vector *command_line, long buff)
{
	int		vct_len;
	t_le	*le;
	le = get_env(GET);
	vct_len = (int)vct_getlen(command_line);

	if (le->vct_index_backup == le->vct_index)
		return; 
	if (le->select_min == -1)
	{
		le->select_min = (le->vct_index < le->vct_index_backup) ? le->vct_index : le->vct_index_backup;
		le->select_max = (le->vct_index < le->vct_index_backup) ? le->vct_index_backup : le->vct_index;
	}
	else if (le->vct_index < le->select_min && le->vct_index < le->select_max)
		le->select_min = le->vct_index;
	else if (le->vct_index > le->select_min && le->vct_index > le->select_max)
		le->select_max = le->vct_index;
	else if (buff == K_LEFT && le->vct_index >= le->select_min && le->vct_index < le->select_max)
		le->select_max = le->vct_index;
	else if (buff == K_RIGHT && le->vct_index > le->select_min && le->vct_index <= le->select_max)
		le->select_min = le->vct_index;

	if (le->select_max >= vct_len)
		le->select_max = vct_len - 1;
}

void	init_selection()
{
	t_le *le;

	le = get_env(GET);
	le->select_min = -1;
	le->select_max = -1;
}

void	unselect_all(t_vector *command_line)
{
	t_le *le;

	le = get_env(GET);
	if (le->select_min != -1)
	{
		init_selection();
		refresh_command_line(command_line);
	}
}
