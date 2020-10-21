#ifndef DEFINE_H
# define DEFINE_H

/******************************************************************************/
/*******************************_LEXER_****************************************/
/******************************************************************************/

#define SEPARATOR		";"//0
#define PIPE			"|"//1
#define SIMPLE_QUOTE	"'"//2
#define QUOTE			"\""//3
#define LESS_THAN		"<"//4
#define GREATER_THAN	">"//5
#define	SPACE			" "//6
#define TAB				"\t"//7
#define DOUBLE_GREATER	">>"//8
#define OR				"||"//9
#define AND				"&&"//10
#define EXP				'$'//12
#define ASSIGN			'='//13

#define S_WORD			"WORD"
#define S_EXP			"$"
#define S_ASSIGN		"="
#define S_START			"START"
#define S_END				"END"
#define S_NO_TYPE			"NO_TYPE"

#define C_SEPARATOR			';'//0
#define C_PIPE				'|'//1
#define C_SIMPLE_QUOTE		'\''//2
#define C_QUOTE				'\"'//3
#define C_LESS_THAN			'<'//4
#define C_GREATER_THAN		'>'//5
#define	C_SPACE				' '//6
#define C_TAB				'\t'//7

#define GRAMMAR			";|'\"<> \t"

#define	NB_GRAMMAR		8
#define DOUBLE_TOKEN	7
#define EXP_ASSIGN		11
#define FIRST_CHAR		0

#define N_SIMPLE_QUOTE	1
#define N_QUOTE			

#define NB_FD			3

/******************************************************************************/
/*******************************_CLEANER_**************************************/
/******************************************************************************/

#define DEL				"-$:/,@+=#%^*~[]{}.?'\" \t"

#define NB_DEL			23		
	
#endif
