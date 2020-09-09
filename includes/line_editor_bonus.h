#include <termcap.h>
#include <termios.h>
#include <term.h>
#include <unistd.h>
#include <string.h>

#include "define_bonus.h"
#include "enum_bonus.h"
#include "lexer_bonus.h"
#include "minishell_bonus.h"
#include "struct_bonus.h"

#include <sys/ioctl.h>

#ifndef LINE_EDITOR_BONUS_H
# define LINE_EDITOR_BONUS_H

/*************************************************
				DEFINES
************************************************/

#ifndef L_ED_BUFF_SIZE
# define	L_ED_BUFF_SIZE	16
#endif

#ifndef DEBUG
# define	DEBUG			FALSE
#endif

#ifndef DEBUG_LLDB
# define	DEBUG_LLDB		FALSE
#endif

#ifndef LLDB
# define	DEBUG_SIGNAL	FALSE
#endif

# define	ERR_NEW_VCT		"Could not malloc vector"
# define	ERR_VCT			"Vector function failed"
# define	ERR_SCREEN_SIZE	"Screen size too small"
# define	ERR_TERM_NAME	"Term environement variable can't be located"
# define	ERR_TERMCAP		"Termcap could not be loaded by termcap library"

# define	K_UP			0x415b1b
# define	K_DOWN			0x425b1b
# define	K_RIGHT			0x435b1b
# define	K_LEFT			0x445b1b

# define	K_SHIFT			0x323b31
# define	K_CTRL			0x323b31

# define	K_CTRL_RIGHT	0x43353b315b1b
# define	K_CTRL_LEFT		0x44353b315b1b

# define	K_END			0x485b1b
# define	K_HOME			0x465b1b

# define	K_DEL_BACKWARD	0x7f
# define	K_DEL_FOREWARD	0x7e335b1b

# define	K_ENTER			0xa
# define	K_ESCAPE		0x1b

# define	PROMPT			"~$>"

# define	NB_TERMCAP		9	

enum	e_termcap
{
	HIDE_CURSOR,
	VISIBLE_CURSOR,
	INSERT_MODE,
	NORMAL_MODE,
	SELECT,
	UNSELECT,
	CLEAR_AFTER_CURSOR,
	SAVE_CURSOR_POS,
	RESTORE_CURSOR_POS
};

/*************************************************
				TYPEDEF
************************************************/

typedef struct	s_line_editor
{
	struct termios	termios_backup;
	char			*termcap[NB_TERMCAP];
	int				srows;
	int				scols;
	int				prompt_len;
	int				cx;
	int				cy;	
	int				vct_index;	
	int				select_min;	
	int				select_max;				
}				t_env;


//t_le	g_le;

/*************************************************
				DEBUG
************************************************/

void	debug_print_infos(t_vector *command_line);
void	debug_print_flag(char *flag);

/*************************************************
				UTILS
************************************************/
//int	get_ctrlkey(char c);
int			ft_iscntrl(char c);
int         ms_putchar(int c);
void		exit_routine_le(char *err_code);
void		handle_esc_seq(char *buff, t_vector *command_line);
void		handle_print_char(char *buff, t_vector *command_line);
void		refresh_command_line(t_vector *command_line);
int			get_ctrlkey(int c);

/*************************************************
				UTILS SELECTION
************************************************/

void		init_selection(void);
void		unselect_all(t_vector *command_line);
void		update_select(void);
int			is_shift_on(long buff);

/*************************************************
				UTILS CURSOR
************************************************/

void		convert_vctindex_cur_pos(int vct_index);
size_t		convert_cur_pos_vctindex(int cx, int cy);
int			move_cursor_right(t_vector *command_line);
int			move_cursor_left(void);

/*************************************************
				INIT FUNCTIONS
************************************************/
void		init_prompt(void);
void		init_term_mode(void);
void		init_termcaps(void);

void		update_window_size(void);

/*************************************************
				MAIN FUNCTIONS
************************************************/
void		line_editor(t_vector *command_line);

#endif
