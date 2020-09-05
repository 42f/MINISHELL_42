#include "minishell_bonus.h"

void	print_prompt(void)
{
	ft_printf("SHELL > ");
}

int	main(void)
{
	t_vector	*vct_input;
	t_vector	*cpy_input;
	
	vct_input = vct_new();
	while (1)
	{
		print_prompt();
		if (vct_readline(vct_input, 0) <= 0)
		{
			/*ici j'utilise vct readline (comme GNL mais avec des vecteurs) car
			il n y a pas encore d'edition de ligne, mais a la place de vct readline
			il faut mettre ta fonction d'edition de ligne (qui utilisera read)
			qui mettra la ligne courante dans vct_input que moi j'utiliserais dans mon lexer*/
			return (EXIT_FAILURE);
		}
		ft_printf("-- La ligne est [%s] --\n", vct_getstr(vct_input)); // DEBUG
		cpy_input = vct_dup(vct_input);
		if (check_quote(cpy_input) == FAILURE)
		{
			ft_printf("FAILURE\n");//DEBUG
			vct_del(&vct_input);
			return (EXIT_FAILURE);
		}
		vct_printendl(vct_input);
		lexer(vct_input); //fonction qui transforme chaque element en token
		// -> in : vecteur que tu me renvoies
		// -> out : list de token
		//ast = parser(token_list); fonction qui permet de voir si token fonctionne ensemble
		//______exemple : SUJET VERBE COMPLEMENT == FONCTIONNE
		//______exemple : SUJET COMPLEMENT VERBRE == FONCTIONNE PAS
		// -> in : list de token
		// -> out : ast (arbre binaire)
		//ast = exp_var(ast); fonction qui permet d'expandre les variables $
		// -> in : ast
		// -> out : ast
		//ast = semiantic(ast); fonction qui permet de voir si la phrase a du sens dans le contexte
		//_____exemple : j'ai 32 ans == FONCTIONNE
		//_____exemple : j'en ai 32 == FONCTIONNE PAS (car aucun contexte)
		// -> in : ast
		// -> out : ast
		// struct (a debattre) = interpretor(ast)
		// execution(struct);
	}
	return (EXIT_SUCCESS);
}
