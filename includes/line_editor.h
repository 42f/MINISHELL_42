/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_editor.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 16:21:22 by bvalette          #+#    #+#             */
/*   Updated: 2020/12/03 16:21:23 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDITOR_H
# define LINE_EDITOR_H

# include <termcap.h>
# include <termios.h>
# include <term.h>
# include <sys/ioctl.h>

# ifndef DEBUG_MODE
#  define DEBUG_MODE FALSE
# endif

# ifndef DEBUG_LLDB
#  define DEBUG_LLDB FALSE
# endif

# ifndef LLDB
#  define DEBUG_SIGNAL FALSE
# endif

/*
*********     UTILS     ***********
*/

# define GET					NULL
# define UNSET					-1
# define RESET					-1

/*
*********  SCREEN  FLAG ***********
*/

# define FULL_REFRESH			0x000001
# define HISTORY_REFRESH		0x000002
# define RESIZE_REFRESH			0x000004

/*
*********  USER KEYS  *************
*/

# define K_EOF					4
# define K_TAB					9
# define K_UP					0x415b1b
# define K_DOWN					0x425b1b
# define K_RIGHT				0x435b1b
# define K_LEFT					0x445b1b
# define K_CTRL_RIGHT			0x43353b315b1b
# define K_CTRL_LEFT			0x44353b315b1b
# define K_END					0x465b1b
# define K_HOME					0x485b1b
# define K_DEL_BACKWARD			0x7f
# define K_DEL_FOREWARD			0x7e335b1b
# define K_ENTER				0xa

# define K_ESCAPE				0x1b
# define K_SHIFT				0x323b31
# define K_CTRL					0x353b31
# define K_CTRL_SHIFT			0x363b31

# define CTRL_MASK				0x000001
# define SHIFT_MASK				0x000002

# define PROMPT_SIMPLE			"NO_LINE_ED~$>"
# define PROMPT_LINE_EDITION	"LINE_EDITION~$>"

/*
*********  DEBUG TERMCAPS *********
*/

# define HIGHLIGHT				"md"
# define NO_HIGHLIGHT			"me"
# define SAVE_CURSOR_POS		"sc"
# define RESTORE_CURSOR_POS		"rc"

/*
*****  LINE EDITOR TERNCAPS ******
*/

# define NB_ESSENTIAL_TERMCAP	12
# define NB_OPTIONAL_TERMCAP	4

enum	e_essential_termcap
{
	CLEAR_LINE,
	CLEAR_ALL_AFTER_CURS,
	SELECT,
	UNSELECT,
	MOVE_CURSOR_HOME,
	ONE_COL_LEFT,
	ONE_COL_RIGHT,
	ONE_ROW_DOWN,
	ONE_ROW_UP,
	MOVE_X_ROWS_UP,
	CURSOR_POSITION_REQUEST,
	RETURN_CARRIAGE
};

enum	e_optional_termcap
{
	MOVE_AT_COL_X = NB_ESSENTIAL_TERMCAP,
	VISIBLE_CURSOR,
	HIDE_CURSOR,
	SCROLLING_CAPABILITY
};

/*
**				TYPEDEF
*/

typedef struct		s_line_editor
{
	int				screen_flag;
	int				srows;
	int				scols;
	int				prompt_len;
	int				cx;
	int				cy;
	int				vct_index;
	int				vct_index_backup;
	struct termios	*termios_editor;
	struct termios	*termios_bkup;
	char			*termcap[NB_ESSENTIAL_TERMCAP + NB_OPTIONAL_TERMCAP];
	char			*cmd_line_backup;
	t_vector		*cmd_line;
	t_vector		*clipboard;
	t_list			*history_cache;
	int				select_min;
	int				select_max;
}					t_le;

/*
**				DEBUG
*/

void				debug_print_infos(void);
void				debug_print_flag(char *flag);

/*
**				UTILS
*/

t_le				*get_struct(t_le *env);
int					ms_putchar(int c);
int					ms_tputs(const char *str, int affcnt, int (*putc)(int));

int					is_shift_on(long key);
int					is_ctrl_on(long key);
int					is_ctrl_shift_on(long key);

/*
**				HANDLERS
*/

void				handle_esc_seq(char key);
void				handle_print_char(char key);

void				insert_char_in_vct(char c);

void				move_end_of_line(void);
void				move_start_of_line(void);
void				move_one_word_right(void);
void				move_one_word_left(void);

void				delete_selection(long key);
void				copy_selection(void);
void				past_clipboard(void);
void				cut_selection(void);

/*
**				HISTORY
*/

void				save_history(void);
void				call_history(long key);
void				free_history(void);

/*
**				SELECTION
*/

void				init_selection(void);
void				unselect_all();
void				update_selection(long key);

/*
**				COMMAND_LINE PRINT
*/

void				refresh_command_line(void);
void				print_cmd_line(void);

/*
**				CURSOR
*/

int					move_cursor_right(void);
int					move_cursor_left(void);
void				move_previous_line_head();
void				move_at_col_x(int target_col);
void				move_cursor_at_index(int index_to);
void				update_cursor_infos(void);

/*
**				INIT FUNCTIONS
*/

void				init_prompt(void);
void				print_prompt(void);
void				init_term_mode(void);
void				set_termios(const struct termios *termios_mode);
t_le				*init_line_editor(t_vector *cmd_line);
void				update_window_size(void);

/*
**				MAIN FUNCTIONS
*/
int					line_editor(void);

#endif
