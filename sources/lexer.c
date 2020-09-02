/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfallet <lfallet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 12:59:04 by lfallet           #+#    #+#             */
/*   Updated: 2020/09/01 21:17:58 by lfallet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	what_token(char c)
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

static void		extract_token(t_list **token_list, char *str, size_t type)
{
	t_token	*token;
	t_list	*node;

	token = (t_token *)malloc(sizeof(t_token));
	node = NULL;
	if (token == NULL)
		exit_routine(token, node);// ERROR
	token->data = NULL;//in the case when it's not a word
	if (type == E_WORD)
		token->data = ft_strdup(str);//in the case it's a word
	token->type = type;
	//ft_printf("token->data = %s\n", token->data); //DEBUG
	//printf("token.type = %u\n\n", token->type); //DEBUG
	node = ft_lstnew(token);//creation of the list de token (structure)
	if (node == NULL)
		exit_routine(token, node);// ERROR
	ft_lstadd_back(token_list, node);//add the list to the node
}

static void		extract_token_word(t_list **token_list, t_vector *vct)
{
	extract_token(token_list, vct_getstr(vct), E_WORD);
	vct_clear(vct);
}

static void		no_word(t_list **token_list, t_vector *word, size_t type)
{
	if (vct_getlen(word) != 0)//if char not word but the c/s before was a word
		extract_token_word(token_list, word);//we extract the word token
	extract_token(token_list, NULL, type);
}

/*void		print_list_debug(t_list *list)
{
	t_token	*token;

	while (list != NULL)
	{
		token = (t_token *)list->content;
		ft_printf("[%d - '%s']%s", token->type, token->data,
				list->next == NULL ? "\n" : " -> ");
		list = list->next;
	}
}*/

t_list			*lexer(t_vector *input)
{
	t_list		*token_list;
	size_t		type;
	t_vector	*word;

	word = vct_new();
	token_list = NULL;
	while (vct_getlen(input) > 0)
	{
		type = what_token(vct_getcharat(input, 0));
		if (type < E_WORD)//if not a word
			no_word(&token_list, word, type);
		else//if it's a word
			vct_add(word, vct_getcharat(input, 0));//add char to buffer word
		vct_pop(input);
	}
	if (vct_getlen(word) != 0)//if the buffer word is not empty
		extract_token_word(&token_list, word);
//	print_list_debug(token_list);
	return (token_list);
}
