#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************/
/*******************************_LEXER_****************************************/
/******************************************************************************/

typedef struct s_lexer
{
	t_list		*token_list;
	ssize_t		type;
	t_vector	*word;
}				t_lexer;

typedef struct s_token
{
	char				*data;
	enum e_token_type	type;
}				t_token;

/******************************************************************************/
/*******************************_PARSER_***************************************/
/******************************************************************************/

typedef struct					s_valid_token
{
	enum e_token_type			*next_token;
}								t_valid_token;

/******************************************************************************/
/*******************************_JOB/COMMAND_**********************************/
/******************************************************************************/

typedef struct				s_cmd
{
	char					*name;
	char					**av;
	int						ac;
	int						fd[NB_FD];
	char					*fd_string[NB_FD];
	char					**env;
	enum e_condition_type	condition;
	int						ret;
	int						redirection;
}
							t_cmd;

typedef struct					s_job
{
	t_list					*cmd_lst;
	int						ret;
}								t_job;

#endif


#endif
