#include "tester.h"

t_lex	g_lexer = lexer;

int		main()
{
	tester("hello|||>>haha|&&", "START#WORD@hello#||#|#>>#WORD@haha#|#&&#END");
	tester("hello=wejfh=", "START#=@hello=wejfh=#END");
	tester("$hellowejfh", "START#$@$hellowejfh#END");
	tester("echo bonjour ; | . 2", "START#WORD@echo#WORD@bonjour#;#|#WORD@.#WORD@2#END");
	tester("echo bonjour | | . 2", "START#WORD@echo#WORD@bonjour#|#|#WORD@.#WORD@2#END");
	tester("echo bonjour ; ls . bonjour ; ls", "START#WORD@echo#WORD@bonjour#;#WORD@ls#WORD@.#WORD@bonjour#;#WORD@ls#END");
	tester("cd $HOME/Bureau .", "START#WORD@cd#$@$HOME/Bureau#WORD@.#END");
	tester("cat /dev/random | head -n 1 | cat -e .", "START#WORD@cat#WORD@/dev/random#|#WORD@head#WORD@-n#WORD@1#|#WORD@cat#WORD@-e#WORD@.#END");
	tester("cat Makefile | grep pr | head -n 5 | cd test (mybin) . 1", "START#WORD@cat#WORD@Makefile#|#WORD@grep#WORD@pr#|#WORD@head#WORD@-n#WORD@5#|#WORD@cd#WORD@test#WORD@(mybin)#WORD@.#WORD@1#END");
	tester("echo bonjour >>> test .", "START#WORD@echo#WORD@bonjour#>>#>#WORD@test#WORD@.#END");
	tester("echo 2 >> out1 > out2 .", "START#WORD@echo#WORD@2#>>#WORD@out1#>#WORD@out2#WORD@.#END");
	tester("echo 2 > out1 >> out2 .", "START#WORD@echo#WORD@2#>#WORD@out1#>>#WORD@out2#WORD@.#END");
	tester(" cat < test .", "START#WORD@cat#<#WORD@test#WORD@.#END");
	tester("cd ../../../../../.. . pwd . /", "START#WORD@cd#WORD@../../../../../..#WORD@.#WORD@pwd#WORD@.#WORD@/#END");
	tester("c$var Makefile (avec $var=at) .", "START#WORD@c$var#WORD@Makefile#WORD@(avec#=@$var=at)#WORD@.#END");
	tester("$LESS$VAR .", "START#$@$LESS$VAR#WORD@.#END");
	tester("echo bonjour > $test w/ t", "START#WORD@echo#WORD@bonjour#>#$@$test#WORD@w/#WORD@t#END");
	tester("echo $PATH", "START#WORD@echo#$@$PATH#END");
	tester("echo hi; echo hihi;", "START#WORD@echo#WORD@hi#;#WORD@echo#WORD@hihi#;#END");
	tester("echo ;;", "START#WORD@echo#;#;#END");
	tester("echo one; echo two; echo three;", "START#WORD@echo#WORD@one#;#WORD@echo#WORD@two#;#WORD@echo#WORD@three#;##END");
	tester("export LOL=lol", "START#WORD@export#=@LOL=lol#END");
	tester("export LOL=lala ROR=rara", "START#WORD@export#=@LOL=lala#=@ROR=rara#END");
	tester("echo $?", "START#WORD@echo#$@$?#END");
	tester("| echo", "START#|#WORD@echo#END");
	tester("echo |", "START#WORD@echo#|#END");
	tester("echo | | echo", "START#WORD@echo#|#|#WORD@echo#END");
	tester("cat < Makefile | grep gcc > output", "START#WORD@cat#<#WORD@Makefile#|#WORD@grep#WORD@gcc#>#WORD@output#END");
	tester("cat < nofile | grep gcc > output // $? 1", "START#WORD@cat#<#WORD@nofile#|#WORD@grep#WORD@gcc#>#WORD@output#WORD@//#$@$?#WORD@1#END");
	tester("if grep christie /etc/passwd > /dev/null  > then ", "START#WORD@if#WORD@grep#WORD@christie#WORD@/etc/passwd#>#WORD@/dev/null#>#WORD@then#END");
	tester("test [-z|-n] $variable", "START#WORD@test#WORD@[-z#|#WORD@-n]#$@$variable#END");
	tester("var1=1+1", "START#=@var1=1+1#END");
	tester("let $var1", "START#WORD@let#$@$var1#END");
	tester("var=$((5*3))", "START#=@var=$((5*3))#END");
	tester("=====", "START#WORD@=====#END");
	tester("=", "START#WORD@=#END");
	tester("coucou==|& &&>>>", "START#=@coucou==#|#WORD@&#&&#>>#>#END");
	tester("&&&&&", "START#&&#&&#WORD@&#END");
	tester("echo $TEST$TEST$TEST", "START#WORD@echo#$@$TEST$TEST$TEST#END");
	tester("export 1TEST= ;", "START#WORD@export#=@1TEST=#;#END");
	tester("export TEST=LOL ; echo $TEST ;", "START#WORD@export#=@TEST=LOL#;#WORD@echo#$@$TEST#;#END");
	tester("export TEST=LOL ; echo $TEST$TEST$TEST=lol$TEST", "START#WORD@export#=@TEST=LOL#;#WORD@echo#=@$TEST$TEST$TEST=lol$TEST#END");
	tester("echo test > ls >> ls >> ls ; echo test >> ls; cat ls", "START#WORD@echo#WORD@test#>#WORD@ls#>>#WORD@ls#>>#WORD@ls#;#WORD@echo#WORD@test#>>#WORD@ls#;#@WORD@cat#WORD@ls#END");
	tester("hello;haha", "START#WORD@hello#;#WORD@haha#END");
	tester("hello prout=hhaa\'\"h\"\"\"ey\"\"\'\"&&||>\"|| hey >> salut", "START#WORD@hello#=@prout=hhaa\'\"h\"\"\"ey\"\"\'\"&&||>\"#||#WORD@hey#>>#WORD@salut#END");
	tester("\"hello\" && echo\'hey\'&& hello= hey=$hey hola=\"\'\'hey\'\'\" he\"haha=hehe\"", "START#WORD@\"hello\"#&&#WORD@echo\'hey\'#&&#=@hello=#=@hey=$hey#=@hola=\"\'\'hey\'\'\"#=@he\"haha=hehe\"#END");
	tester("\"hello=hey\"", "START#=@\"hello=hey\"#END");
	tester("\"hello&&hey\"", "START#WORD@\"hello&&hey\"#END");
	tester("export \"test=ici\"=coucou", "START#WORD@export#=#\"test=ici\"=coucou#END");
	tester("export \'\"hellohey\"\'=446", "START#WORD@export#=@\'\"hellohey\"\'=446#END");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	//tester("", "");
	return (EXIT_SUCCESS);
}
