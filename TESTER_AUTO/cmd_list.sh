#! /bin/bash --posix

print_separator(){
	i=1
	max=$(tput cols)
	while [ $i -le $max ]
	do
		echo -n $1
		((i+=1))
	done
	echo
}

print_diff_full () {
	colordiff $EXTRA_FLAGS -y /tmp/ba.log /tmp/minishell.log
}

print_diff_fail () {
	echo -e "\e[31m \e[1m[$TEST_NB][  ] \e[0m\t\t["$TEST"]"
	echo -e "🟨 [Diff]\n"
	diff $EXTRA_FLAGS /tmp/ba.log /tmp/minishell.log | cat -e
	echo -e "\n🟡 [Color Diff]\n"
	print_diff_full
}

print_diff_simple (){
	# SORT_RESULT allow to sort lines in order to avoid race conditions false positive
	# not set by default
	if [[ ! -z $SORT_RESULT ]]
	then
		echo -e "\n \e[36m ℹ️ This test sort the output in order to avoid race condition :\e[0m"
		cat /tmp/ba.log | sort > /tmp/ba_sorted.log ; cat /tmp/ba_sorted.log > /tmp/ba.log
		cat /tmp/minishell.log | sort > /tmp/ms_sorted.log ; cat /tmp/ms_sorted.log > /tmp/minishell.log
	fi
	diff $EXTRA_FLAGS -s /tmp/ba.log /tmp/minishell.log &>/dev/null
	if [[ $? -ne 0 ]]
	then
		print_separator '▁'
		print_diff_fail
		((TEST_FAILED_NB+=1))
		echo "🔴 [$TEST_NB] $TEST" >> /tmp/test_ko
		echo -e "\e[34m $(diff $EXTRA_FLAGS -s /tmp/ba.log /tmp/minishell.log)\e[0m" >> /tmp/test_ko
		echo -e "\t ⏫ [$TEST_NB][$TEST]" >> /tmp/bash_sumup
		echo -e "\t ⏫ [$TEST_NB][$TEST]" >> /tmp/minishell_sumup
		print_separator '▔'
	else
		echo -e "\e[32m \e[1m[$TEST_NB][OK] \e[0m\t\t["$TEST"]"
	fi
}

clean_log () {
## bricolage destiné a retirer les faux negatifs du testeur
	sed -i 's/NO_LINE_ED~$>//g' /tmp/minishell.log
	sed -i 's/Minishell: /bash: /g' /tmp/minishell.log
	sed -i 's/minishell: /bash: /g' /tmp/minishell.log
	# sed -i 's/bash-4.4$ exit//' /tmp/ba.log

	#to remove once fixed
	cat /tmp/ba.log | grep -v "\_\=\|bash-4.4\\$ " > /tmp/ba_tmp.log ; cat /tmp/ba_tmp.log > /tmp/ba.log

	#to remove once fixed
	# cat /tmp/minishell.log | grep -v "exit$" > /tmp/minishell_tmp.log ; cat /tmp/minishell_tmp.log > /tmp/minishell.log
}

test () {
	((TEST_NB+=1))
	export TEST=$1

	echo "$TEST" | env -i $EXTRA_ENV ./Minishell &>/tmp/minishell.log #; echo "RETURNED : $?" >> /tmp/minishell.log
	echo "$TEST" | env -i $EXTRA_ENV bash --posix -i &>/tmp/ba.log #; echo "RETURNED : $?" >> /tmp/ba.log
	# echo "$TEST" | env -i $EXTRA_ENV ./Minishell &>/tmp/minishell.log
	# echo "RETURNED : $?" >> /tmp/minishell.log
	# echo "$TEST" | env -i $EXTRA_ENV bash --posix -i &>/tmp/ba.log
	# echo "RETURNED : $?" >> /tmp/ba.log

	echo -e "\n\n\t 🟡 [$TEST_NB][$TEST] 🟡 " >> /tmp/bash_sumup
	cat /tmp/ba.log >> /tmp/bash_sumup
	echo -e "\n\n\t 🟡 [$TEST_NB][$TEST] 🟡 " >> /tmp/minishell_sumup
	cat /tmp/minishell.log >> /tmp/minishell_sumup

	clean_log
	print_diff_simple
}

test_random() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"


	test "unset SHSLVL PATH PWD OLDPWD _ ; echo \$PWD ; pwd ; ls"
	test "hfdjskhdfkjhfsd"
	test "ls fdsfsdfhfsd"
	test "echo aaaaa bbbb cccccc dddddd > /tmp/a ; cat -e /tmp/a"
	test "echo terminal is [\$TERM]"
	test "echo terminal is [\$TERM4"
	test " toto+=42; echo \$toto ; toto+=.; echo \$toto ; toto+=-; echo \$toto ; toto+='' ; echo \$toto ; toto+=42; echo \$toto ; toto+=42; echo \$toto ; toto+=42; echo \$toto ; toto+=42; echo \$toto"
	test "echo > /tmp/test"
	test "cat -e /tmp/test"
	test "echo -n > /tmp/test"
	test "cat -e /tmp/test"
	test "echo \$?"
	test "ls hdfjkdsf ; echo \$?"
	test "ls hdfjkdsf ; echo \$?"
	test "export TEST; export | grep TEST"
	test "export TEST=1; export | grep TEST"
	test "export VAR_VALID=1"
	test "export BROK;EN_VAR=1; export | grep EN_VAR"
	test "export TEST+=23; export | grep TEST"
	test "export TEST1 TEST2=456 TEST4 TEST5 TEST3=78"
	test " export TEST1=123; export TEST1 TEST2=456 TEST4 TEST5 TEST3=789		;
echo \$TEST\$TEST2\$TEST3 > /tmp/test1 > /tmp/test2 > /tmp/test3	;
cat /tmp/test1; cat /tmp/test2; cat /tmp/test3 "
	test "cd .././../.././../bin/ls"
	test "unset \$HOME; cd"
	test "touch /tmp/file ; /tmp/file"
	test "4ABC=toto"
}

test_failed() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "	\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\";\"	"
	test " export EMPTY ;
export NOTEMPTY= ;
export CHARS=AAA ;
echo '\$CHARS' ;
echo \$CHARS ;
echo \$PWD\$HOMe\"\$VAR_NONEXISTANT\$PWD\" \$NOTEMPTY\$EMPTY'' \$\"VAR_NONEXISTANT\"'\$EMPTY\$\"PWD' \$CHARS\"\$CHARS\"'\$PWD\"\$PWD\"'\$EMPTY\$NOTEMPTY |wc -m ;"
	test "
		export EMPTY ;
		export NOTEMPTY= ;
		export CHARS=AAA ;
		echo '\$CHARS' ;
		echo \$CHARS ;
		echo \$PWD\$VAR_NONEXISTANT\"\$VAR_NONEXISTANT\$PWD\" \$NOTEMPTY\$EMPTY'' \$\"VAR_NONEXISTANT\"'\$EMPTY\$\"PWD' \$CHARS\"\$CHARS\"'\$PWD\"\$PWD\"'\$EMPTY\$NOTEMPTY ;
	"
	test "
		echo \"\$HOME\"
		echo \$HOME
		echo \"\$\"HOME
		echo \$\"\"HOME
		echo \"\"\$HOME
		echo \$\"HOME\"
		echo \$\"HO\"\"ME\"
		echo \"\$HO\"\"ME\"
		echo \"\$HO\"ME
		echo \$\"HOME\"
	"
	test "
		mkdir -p test1/test2/test3
		cd test1/test2/test3
		rm -r ../../../test1
		cd ..
		pwd
		cd ..
		pwd
		cd ..
		pwd
	"
	test "export 4ABC=toto"
	test "toto/tata=1"
}

test_bonus () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "\"\""
	test "\'\'"
	test "echo a && echo b ; echo \$?"
	test "echo a || echo b ; echo \$?"
	test "false || echo b ; echo \$?"
	test "false && echo b ; echo \$?"
	test "echo a || false ; echo \$?"
	test "echo a && false ; echo \$?"
}

test_correction_exec () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "/bin/ls"
	test "/bin/ip"
	test "/bin/aaaaaaaaaaaa"
	test "./non_existing_file______pouette"
	test "./fail_bin/segfault"
	test "non_existing_command______pouette"
	test "./Makefile"
	test "/dev"
	test "../../../ls"
	test "\"\""
	test ""
	test " "


 }

test_correction_arg () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "/bin/ls -l"
	test "/bin/ls aaaaaaaaaaa"
	test "/bin/ls -x Makefile"
	test "/bin/ls - Makefile"
	test "/bin/ls -- Makefile"
}


test_correction_echo () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "echo echo echo echo"
	test "echo ; echo ; echo ; echo"
	test "echo coucou a b c d e f g"
	test "echo - test"
	test "echo -nnnnnnx hello"
	test "echo -----nnnnnnn hello"
	test "echo - hello"
	test "echo - nnnnnnn hello"
	test "echo - n hello"
	test "echo -n"
	test "echo -n -n -n -n"
	test "echo -nnnn"

	test "echo $ ; echo "$" ; echo '$'"
    test "echo $"" ; echo "$""" ; echo '$'''"
    test "echo \$toto ; echo \"\$toto\" ; echo '\$toto'"
    test "echo \$toto\"\" ; echo \"\$toto\"\"\" ; echo '\$toto'''"
    test "toto= 42 ; echo \$toto ; echo \"\$toto\" ; echo '\$toto'"
    test "toto=42 ; echo \$toto\"\" ; echo \"\$toto\"\"\" ; echo '\$toto'''"

# Cannot be tested with this
	# extra flags to void tests to fail because of files redirection and new lines
	# EXTRA_FLAGS="--ignore-trailing-space"
	# EXTRA_FLAGS="--ignore-all-space"
		# test "echo -n test aaa"
		# test "echo -n -n -n -n -n -n test aaa"
		# test "echo -n -n -N -n -n -n test aaa"
		# test "echo n -n -n -n -n -x test aaa"
	# EXTRA_FLAGS=""
}


test_correction_exit () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "exit 5"
	test "exit abcdef"
	test "exit 2 2 2 2 2 2"
	test "exit dqdsqd 2 dsqdqs"
	test "exit 9223372036854775807"
	test "exit 9223372036854775808"
	test "exit 500"
	test "exit -500"
	test "ls ; exit"
	test "ls | exit"
}


test_correction_return () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "/bin/ip ; echo \$?"
	test "/bin/ipaaaaaaaaaa ; echo \$?"
	test "/bin/ls aaaaaaabbbbbbbb ; echo \$?"
	test "/bin/ls aaaaaaaaaaa ; echo \$?"
	test "/bin/ls -x Makefile ; echo \$?"
	test "/bin/ls - Makefile ; echo \$?"
	test "/bin/ls -- Makefile ; echo \$?"
}


test_correction_semicolons () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "echo a ; echo b"
	test "echo a; echo b"
	test "echo a ;echo b"
	test "echo a;echo b"
	test "echo a' ; 'echo b"
	test "echo a'; 'echo b"
	test "echo a' ;'echo b"
	test "echo a';'echo b"
	test "echo a ';' echo b"
}


test_correction_baskslashs () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "ls \\Makefile"
	test "ls \\\"Makefile\\\""
	test "ls \\\'Makefile\\\'"
}


test_correction_env () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "env | sort "
	test "cat=meow ; env  | sort ; export"
	test "export cat ; env  | sort ; export"
	test "export cat=42 ; env  | sort ; export"
	test "unset PWD ; cat=meow env; echo ; env | sort  ; export"
	test "unset PWD ; cat=meow env; echo ; env | sort ; export"
	test "cat env | sort; env | sort ; export"
	test "cat=meow env | sort; env | sort  ; export"
	test "cat=meow env | sort; env | sort ; export"
	test "cat env | sort; env | sort ; export"
	test "export hello=hey ; echo \$? ; export -hola=touto ; echo \$? ; env -l ; echo \$? ; unset hola ; echo \$? ; unset hello ; echo \$? ; export toto+=hello ; echo \$? ; export _titi=tutu ; echo \$?"

}

test_correction_export_identifier () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "export  _cat  ; export"
	test "export  %cat  ; export"
	test "export  4cat  ; export"
	test "export  ca_t  ; export"
	test "export  ca%t  ; export"
	test "export  ca4t  ; export"

	test "export abcd.  ; export"
	test "export abcd/  ; export"
	test "export abcd]  ; export"
	test "export abcd[  ; export"
	test "export abcd;  ; export"
	test "export abcd:  ; export"
	test "export abcd,  ; export"
	test "export abcd@  ; export"
	test "export abcd%  ; export"
	test "export abcd^  ; export"
	test "export abcd!  ; export"
	test "export abcd?  ; export"
	# test "export abcd+  ; export"

	test "export .  ; export"
	test "export /  ; export"
	test "export ]  ; export"
	test "export [  ; export"
	test "export ;  ; export"
	test "export :  ; export"
	test "export ,  ; export"
	test "export @  ; export"
	test "export %  ; export"
	test "export ^  ; export"
	test "export !  ; export"
	test "export ?  ; export"
	# test "export +  ; export"

	test "export  _cat=42  ; export"
	test "export  %cat=42  ; export"
	test "export  4cat=42  ; export"
	test "export  ca_t=42  ; export"
	test "export  ca%t=42  ; export"
	test "export  ca4t=42  ; export"

	test "export abcd.=42  ; export"
	test "export abcd/=42  ; export"
	test "export abcd]=42  ; export"
	test "export abcd[=42  ; export"
	test "export abcd;=42  ; export"
	test "export abcd:=42  ; export"
	test "export abcd,=42  ; export"
	test "export abcd@=42  ; export"
	test "export abcd%=42  ; export"
	test "export abcd^=42  ; export"
	test "export abcd!=42  ; export"
	test "export abcd?=42  ; export"
	# test "export abcd+=42  ; export"

	test "export .=42  ; export"
	test "export /=42  ; export"
	test "export ]=42  ; export"
	test "export [=42  ; export"
	test "export ;=42  ; export"
	test "export :=42  ; export"
	test "export ,=42  ; export"
	test "export @=42  ; export"
	test "export %=42  ; export"
	test "export ^=42  ; export"
	test "export !=42  ; export"
	test "export ?=42  ; export"
	# test "export +=42  ; export"

}

test_correction_export_identifier_mix_valid () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "export food=pizza  _cat  ; export"
	test "export food=pizza  %cat  ; export"
	test "export food=pizza  4cat  ; export"
	test "export food=pizza  ca_t  ; export"
	test "export food=pizza  ca%t  ; export"
	test "export food=pizza  ca4t  ; export"

	test "export food=pizza abcd.  ; export"
	test "export food=pizza abcd/  ; export"
	test "export food=pizza abcd]  ; export"
	test "export food=pizza abcd[  ; export"
	test "export food=pizza abcd;  ; export"
	test "export food=pizza abcd:  ; export"
	test "export food=pizza abcd,  ; export"
	test "export food=pizza abcd@  ; export"
	test "export food=pizza abcd%  ; export"
	test "export food=pizza abcd^  ; export"
	test "export food=pizza abcd!  ; export"
	test "export food=pizza abcd?  ; export"

	test "export food=pizza .  ; export"
	test "export food=pizza /  ; export"
	test "export food=pizza ]  ; export"
	test "export food=pizza [  ; export"
	test "export food=pizza ;  ; export"
	test "export food=pizza :  ; export"
	test "export food=pizza ,  ; export"
	test "export food=pizza @  ; export"
	test "export food=pizza %  ; export"
	test "export food=pizza ^  ; export"
	test "export food=pizza !  ; export"
	test "export food=pizza ?  ; export"

	test "export food=pizza  _cat=42  ; export"
	test "export food=pizza  %cat=42  ; export"
	test "export food=pizza  4cat=42  ; export"
	test "export food=pizza  ca_t=42  ; export"
	test "export food=pizza  ca%t=42  ; export"
	test "export food=pizza  ca4t=42  ; export"

	test "export food=pizza abcd.=42  ; export"
	test "export food=pizza abcd/=42  ; export"
	test "export food=pizza abcd]=42  ; export"
	test "export food=pizza abcd[=42  ; export"
	test "export food=pizza abcd;=42  ; export"
	test "export food=pizza abcd:=42  ; export"
	test "export food=pizza abcd,=42  ; export"
	test "export food=pizza abcd@=42  ; export"
	test "export food=pizza abcd%=42  ; export"
	test "export food=pizza abcd^=42  ; export"
	test "export food=pizza abcd!=42  ; export"
	test "export food=pizza abcd?=42  ; export"

	test "export food=pizza .=42  ; export"
	test "export food=pizza /=42  ; export"
	test "export food=pizza ]=42  ; export"
	test "export food=pizza [=42  ; export"
	test "export food=pizza ;=42  ; export"
	test "export food=pizza :=42  ; export"
	test "export food=pizza ,=42  ; export"
	test "export food=pizza @=42  ; export"
	test "export food=pizza %=42  ; export"
	test "export food=pizza ^=42  ; export"
	test "export food=pizza !=42  ; export"
	test "export food=pizza ?=42  ; export"

	test "export  _cat  food=pizza ; export"
	test "export  %cat  food=pizza ; export"
	test "export  4cat  food=pizza ; export"
	test "export  ca_t  food=pizza ; export"
	test "export  ca%t  food=pizza ; export"
	test "export  ca4t  food=pizza ; export"
	test "export abcd.  food=pizza ; export"
	test "export abcd/  food=pizza ; export"
	test "export abcd]  food=pizza ; export"
	test "export abcd[  food=pizza ; export"
	test "export abcd;  food=pizza ; export"
	test "export abcd:  food=pizza ; export"
	test "export abcd,  food=pizza ; export"
	test "export abcd@  food=pizza ; export"
	test "export abcd%  food=pizza ; export"
	test "export abcd^  food=pizza ; export"
	test "export abcd!  food=pizza ; export"
	test "export abcd?  food=pizza ; export"
	test "export .  food=pizza ; export"
	test "export /  food=pizza ; export"
	test "export ]  food=pizza ; export"
	test "export [  food=pizza ; export"
	test "export ;  food=pizza ; export"
	test "export :  food=pizza ; export"
	test "export ,  food=pizza ; export"
	test "export @  food=pizza ; export"
	test "export %  food=pizza ; export"
	test "export ^  food=pizza ; export"
	test "export !  food=pizza ; export"
	test "export ?  food=pizza ; export"
	test "export  _cat=42  food=pizza ; export"
	test "export  %cat=42  food=pizza ; export"
	test "export  4cat=42  food=pizza ; export"
	test "export  ca_t=42  food=pizza ; export"
	test "export  ca%t=42  food=pizza ; export"
	test "export  ca4t=42  food=pizza ; export"
	test "export abcd.=42  food=pizza ; export"
	test "export abcd/=42  food=pizza ; export"
	test "export abcd]=42  food=pizza ; export"
	test "export abcd[=42  food=pizza ; export"
	test "export abcd;=42  food=pizza ; export"
	test "export abcd:=42  food=pizza ; export"
	test "export abcd,=42  food=pizza ; export"
	test "export abcd@=42  food=pizza ; export"
	test "export abcd%=42  food=pizza ; export"
	test "export abcd^=42  food=pizza ; export"
	test "export abcd!=42  food=pizza ; export"
	test "export abcd?=42  food=pizza ; export"
	test "export .=42  food=pizza ; export"
	test "export /=42  food=pizza ; export"
	test "export ]=42  food=pizza ; export"
	test "export [=42  food=pizza ; export"
	test "export ;=42  food=pizza ; export"
	test "export :=42  food=pizza ; export"
	test "export ,=42  food=pizza ; export"
	test "export @=42  food=pizza ; export"
	test "export %=42  food=pizza ; export"
	test "export ^=42  food=pizza ; export"
	test "export !=42  food=pizza ; export"
	test "export ?=42  food=pizza ; export"

}

test_correction_export () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"


	test "export ; env | grep cat"
	test "cat=MEOOW ; export cat; export | grep cat ; env | grep cat"
	test "cat='' ; export cat ; export | grep cat ; env | grep cat"
	test "cat=\"\" ; export cat ; export | grep cat ; env | grep cat"
	test "cat='\"\"' ; export cat ; export | grep cat ; env | grep cat"
	test "cat=\"''\" ; export cat ; export | grep cat ; env | grep cat"
	test "cat='\"''\"' ; export cat ; export | grep cat ; env | grep cat"
	test "export cat ; export | grep cat ; env | grep cat"
	test "export cat=''; export | grep cat ; env | grep cat"
	test "export cat='0'; export | grep cat ; env | grep cat"
	test "export cat=\"\" ; export | grep cat ; env | grep cat"
	test "export cat= export | grep cat ; env | grep cat"
	test "export cat= ; export | grep cat ; env | grep cat"

	test "cat=meow export food=pizza ; export ; export"
	test "export -toto=1"
	test "export cat=woof -toto=1"
	test "export -xxxxx cat=woof"
	test "export -xxxxx cat=woof -yyyyyy"
	test "export cat=meow ; echo \$cat"
	test "food=pizza export; export"
	test "export cat=meow ; export cat=woof ; export"
	test "export cat=meow ; cat=woof ; export"
	test "cat=meow ; export cat=woof ; export"
	test "cat=meow ; export cat=woof export ; export"
	test "cat+=woof ; cat+=woof ; cat+=piaou ; export cat; export"
	test "export cat=woof ; cat+=woof ; cat+=piaou ; export"
	test "export cat+=woof ; cat+=woof ; cat+=piaou ; export"
	test "export ERR+EUR=1 ; export"
	test "export VAR-INVALID=1 ; export"
	test "export PATH=42 ; export"
	test "export ; export"
	test "cat=moew export | export"
	test "export cat=moew | export"
	test "cat=moew export"
	test "export cat=moew ; export"
	test "unset PATH ; export PATH ; export ; ls"
	test "toto=42 ; echo \$? ; export to%to; echo \$? ; export"
	test "toto=42 export to%to; echo \$? ; export"
	test "toto= 42 export toto+=hello ; echo \$toto ; echo \$? ; unset toto ; echo \$toto"

}

test_correction_unset_identifier () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "unset ."
	test "unset /"
	test "unset ]"
	test "unset ["
	test "unset ;"
	test "unset :"
	test "unset ,"
	test "unset @"
	test "unset +"
	test "unset %"
	test "unset ^"
	test "unset !"
	test "unset ?"

	test "unset cat=."
	test "unset cat=/"
	test "unset cat=]"
	test "unset cat=["
	test "unset cat=;"
	test "unset cat=:"
	test "unset cat=,"
	test "unset cat=@"
	test "unset cat=+"
	test "unset cat=%"
	test "unset cat=^"
	test "unset cat=!"
	test "unset cat=?"

	test "unset cat=\"\""
	test "unset cat=''"
	test "unset cat=\"''\""
	test "unset cat='\"\"'"

	test "unset abcd."
	test "unset abcd/"
	test "unset abcd]"
	test "unset abcd["
	test "unset abcd;"
	test "unset abcd:"
	test "unset abcd,"
	test "unset abcd@"
	test "unset abcd+"
	test "unset abcd%"
	test "unset abcd^"
	test "unset abcd!"
	test "unset abcd?"

	test "unset _cat"
	test "unset %cat"
	test "unset 4cat"
	test "unset ca_t"
	test "unset ca%t"
	test "unset ca4t"

	test "unset _cat=wwooff"
	test "unset cat=_wwooff"
	test "unset %cat=wwooff"
	test "unset cat=%wwooff"
	test "unset cat=wwooff"
	test "unset cat=%wwooff"
	test "unset 4cat=wwooff"
	test "unset cat=4wwooff"

	test "unset c+at=wwooff"
	test "unset cat=w+wooff"
	test "unset ca%t=wwooff"
	test "unset cat=wwoo%ff"
	test "unset ca_t=wwooff"
	test "unset cat=wwo_off"
	test "unset ca4t=wwooff"
	test "unset cat=wwo4off"
}

test_correction_unset_identifier_mix_valid () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "export food=pizza ; unset . food ; export | grep food"
	test "export food=pizza ; unset / food ; export | grep food"
	test "export food=pizza ; unset ] food ; export | grep food"
	test "export food=pizza ; unset [ food ; export | grep food"
	test "export food=pizza ; unset ; food ; export | grep food"
	test "export food=pizza ; unset : food ; export | grep food"
	test "export food=pizza ; unset , food ; export | grep food"
	test "export food=pizza ; unset @ food ; export | grep food"
	test "export food=pizza ; unset + food ; export | grep food"
	test "export food=pizza ; unset % food ; export | grep food"
	test "export food=pizza ; unset ^ food ; export | grep food"
	test "export food=pizza ; unset ! food ; export | grep food"
	test "export food=pizza ; unset ? food ; export | grep food"
	test "export food=pizza ; unset cat=. food ; export | grep food"
	test "export food=pizza ; unset cat=/ food ; export | grep food"
	test "export food=pizza ; unset cat=] food ; export | grep food"
	test "export food=pizza ; unset cat=[ food ; export | grep food"
	test "export food=pizza ; unset cat=; food ; export | grep food"
	test "export food=pizza ; unset cat=: food ; export | grep food"
	test "export food=pizza ; unset cat=, food ; export | grep food"
	test "export food=pizza ; unset cat=@ food ; export | grep food"
	test "export food=pizza ; unset cat=+ food ; export | grep food"
	test "export food=pizza ; unset cat=% food ; export | grep food"
	test "export food=pizza ; unset cat=^ food ; export | grep food"
	test "export food=pizza ; unset cat=! food ; export | grep food"
	test "export food=pizza ; unset cat=? food ; export | grep food"
	test "export food=pizza ; unset abcd. food ; export | grep food"
	test "export food=pizza ; unset abcd/ food ; export | grep food"
	test "export food=pizza ; unset abcd] food ; export | grep food"
	test "export food=pizza ; unset abcd[ food ; export | grep food"
	test "export food=pizza ; unset abcd; food ; export | grep food"
	test "export food=pizza ; unset abcd: food ; export | grep food"
	test "export food=pizza ; unset abcd, food ; export | grep food"
	test "export food=pizza ; unset abcd@ food ; export | grep food"
	test "export food=pizza ; unset abcd+ food ; export | grep food"
	test "export food=pizza ; unset abcd% food ; export | grep food"
	test "export food=pizza ; unset abcd^ food ; export | grep food"
	test "export food=pizza ; unset abcd! food ; export | grep food"
	test "export food=pizza ; unset abcd? food ; export | grep food"
	test "export food=pizza ; unset _cat food ; export | grep food"
	test "export food=pizza ; unset %cat food ; export | grep food"
	test "export food=pizza ; unset 4cat food ; export | grep food"
	test "export food=pizza ; unset ca_t food ; export | grep food"
	test "export food=pizza ; unset ca%t food ; export | grep food"
	test "export food=pizza ; unset ca4t food ; export | grep food"
	test "export food=pizza ; unset _cat=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=_wwooff food ; export | grep food"
	test "export food=pizza ; unset %cat=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=%wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=%wwooff food ; export | grep food"
	test "export food=pizza ; unset 4cat=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=4wwooff food ; export | grep food"
	test "export food=pizza ; unset c+at=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=w+wooff food ; export | grep food"
	test "export food=pizza ; unset ca%t=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=wwoo%ff food ; export | grep food"
	test "export food=pizza ; unset ca_t=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=wwo_off food ; export | grep food"
	test "export food=pizza ; unset ca4t=wwooff food ; export | grep food"
	test "export food=pizza ; unset cat=wwo4off food ; export | grep food"
}

test_correction_unset () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "unset"
	test "unset hfdjskhdfkjhfsd ; env | sort"
	test "export cat ; unset cat ; echo \$? ; export"
	test "export cat=meow ; unset cat ; echo \$? ; export"
	test "export food=pizza; export | grep food ; unset -xxxxxxx -yyyyy food ; echo \$?"
	test "export food=pizza; export | grep food ; unset -xxxxxxx ; echo \$?"
	test "export food=pizza; export | grep food ; unset -xxxxxxx food ; echo \$? ; export "
	test "export food=pizza; export | grep food ; unset -xxxxxxx food -yyyyyy ; echo \$? ; export"
	test "export food=pizza; export | grep food ; unset food -xxxxxxx ; echo \$? ; export"
	test "export food=pizza cat=MEOOWW; export | grep 'food\|cat' ; unset food -xxxxxxx cat ; echo \$? ; export"
	test "export food=pizza cat=MEOOWW; export | grep 'food\|cat' ; unset food -xxxxxxx cat -yyyyyy ; echo \$? ; export"
	test "export food=pizza; export | grep food ; cat=MEOOOWWW unset food ; export"
	test "export food=pizza; export | grep food ; cat=MEOOOWWW unset food cat ; export"
	test "export food=pizza; export | grep food ; cat=MEOOOWWW unset cat ; export"
	test "export food=pizza; export | grep food ; cat=MEOOOWWW unset food | ls ; export"

	test "food=pizza; env | sort | grep food ; unset -xxxxxxx -yyyyy food ; echo \$?"
	test "food=pizza; env | sort | grep food ; cat=MEOOOWWW unset food | ls ; env | sort"
	test "food=pizza; env | sort | grep food ; food=MEOOOWWW unset food | ls ; env | sort"
	test "food=pizza; env | sort | grep food ; cat=MEOOOWWW unset food ; env | sort"
	test "food=pizza; env | sort | grep food ; food=MEOOOWWW unset food ; env | sort"

	test "dog=MEOWWW unset a b c d e ; export ; env | sort"
	test "dog=MEOWWW unset dog ; export ; env | sort"
}

test_correction_exp () {
	print_separator '█'

	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"
	test "echo \$TERM"
	test "echo \$\"TERM\""
	test "echo \$\'TERM\'"
	test "export TOTO=42 ; echo \$TERM\$TOTO"
	test "echo \$TERMaaaaaaa"
	test "echo aaaaaaa\$TERM"
	test "echo -\$TERM"
	test "cmd=\"ls\" ; \$cmd"
}

test_correction_cd() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "cd ../ ; /bin/ls"
	test "cd ../././../// ; /bin/ls"
	test "cd / ; /bin/ls"
	test "cd ./ ; /bin/ls"
	test "cd ./.. ; /bin/ls"
	test "cd ./sources ; /bin/ls"
	test "cd ./aaaaaaaaaa ; /bin/ls"
	test "cd /aaaaaaaaaa ; /bin/ls"
	test "cd aaaaaaa ; /bin/ls"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa ; cd /tmp/aaa ; /bin/ls"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa ; chmod 000 /tmp/aaa ; cd /tmp/aaa ; /bin/ls"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa ; cd /tmp/aaa ; rm -rf /tmp/aaa ; cd .. ; /bin/ls"
	test "cd /tmp ; cd - ; /bin/ls"
	test "cd Makefile"
	test "cd -x"
}

test_correction_pwd() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "pwd"
	test "pwd -X"
	test "pwd aaaaaaaaa"
	test "pwd ./aaaaaaaaa"
	test "cd ../ ; pwd"
	test "cd ../././../// ; pwd"
	test "cd / ; pwd"
	test "cd ./ ; pwd"
	test "cd ./ ; pwd"
	test "cd ./sources ; pwd"
	test "cd ./aaaaaaaaaa ; pwd"
	test "cd aaaaaaa ; pwd"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa ; cd /tmp/aaa ; pwd"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa ; cd /tmp/aaa ; chmod 000 /tmp/aaa ; pwd"
	test "rm -rf /tmp/aaa ; mkdir -p /tmp/aaa/bbb ; cd /tmp/aaa/bbb ; rm -rf /tmp/aaa ; pwd"
	test "cd /tmp ; cd - ; pwd"
}

test_correction_PATH() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "unset PATH; \"\""
	test "unset PATH; hjsdfkhfds"
	test "unset PATH; ls"
	test "unset PATH; Makefile"
	test "unset PATH; /bin/ls"
	test "export PATH=\"\"; ls"
	test "export PATH= ; ls"
	test "export PATH=pizza ; ls"
	test "export PATH=pizza ; pwd"
	test "export PATH=pizza ; echo"
	test "PATH=/tmp:./ ; ls"
}

test_correction_simple_quotes(){
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "ls ''"
	test "ls ''''"
	test "echo ''"
	test "echo $''"
	test "export toto='' ; export"
	test "''"
	test "cat ''"
}

test_correction_redirect(){
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "ls /tmp > /tmp/file ; cat /tmp/file ; echo \$?"
	test "date >../../../../../../../../../../../../tmp/file2 ; cat /tmp/file2 ; echo \$?"
	test "ls-l /tmp > /tmp/ls ; cat </tmp/ls ; echo \$?"
	test "ls > /tmp/ls ; pwd >> /tmp/ls ; cat </tmp/ls ; echo \$?"
	test "echo ls > /tmp/ls ; cat < /tmp/ls ; cat /tmp/ls ; echo \$?"
	test "cat </tmp ; echo \$?"
	test "cat >/tmp ; echo \$?"
	test "touch /tmp/aaaaaaaa >/tmp ; ls -l /tmp/aaaaaaaaa ; echo \$?"
	test "touch /tmp/aaaaaaaa </tmp ; ls -l /tmp/aaaaaaaaa ; echo \$?"
	test "rm -rf /tmp/b ; ls >> /tmp/b ; ls >> /tmp/b ; ls >> /tmp/b ; ls >> /tmp/b ; cat /tmp/b ; echo \$?"
	test "rm -rf /tmp/b ; ls >> /tmp/b ; ls >> /tmp/b ; ls >> /tmp/b ; ls > /tmp/b ; cat /tmp/b ; echo \$?"
	test "cp Makefile /tmp/b ; true > /tmp/b ; cat </tmp/b ; echo \$?"
	test "cat > coucou > test_cat < coucou | cat < coucou ; rm -rf coucou test_cat ; echo \$?"
	test "rm -rf /tmp/a ; ls >/tmp/a </tmp ; ls /tmp/a ; echo \$?"
	test "rm -rf /tmp/a ; touch /tmp/a >>/tmp ; ls -l /tmp/a ; echo \$?"
	test "rm -rf /tmp/a ; touch /tmp/a >/tmp ; ls -l /tmp/a ; echo \$?"
	test "rm -rf /tmp/a /tmp/b /tmp/c ; echo aaa >        /tmp/a>     /tmp/b >                   /tmp/c ; ls -l /tmp/a /tmp/b /tmp/c ;  cat /tmp/a /tmp/b /tmp/c ; echo \$?"
	test "rm -rf /tmp/a /tmp/b /tmp/c ; echo aaa >/tmp/a >/tmp/b >/tmp/c ; ls -l /tmp/a /tmp/b /tmp/c ;  cat /tmp/a /tmp/b /tmp/c ; echo \$?"
	test "rm -rf /tmp/a ; touch /tmp/a >/tmp >/tmp >/ ; ls -l /tmp/a ; rm -rf /tmp/a ; echo \$?"
	test "cal >/tmp ; echo \$?"
	test "ls >/tmp >/tmp >/ ; echo \$?"
	test "ls >/tmp >/tmp >/ | fakecommande ; echo \$?"
	test "ls /tmp >/tmp >/ | cut -b 1-2 ; echo \$?"
	test "echo aaaaaaaaa >/tmp >/tmp >/ ; echo \$?"
	test "echo aaaaaaaaa >/tmp >/tmp >/ | fakecommande ; echo \$?"
	test "echo aaaaaaaaa >/tmp >/tmp >/ | cut -b 1-2 ; echo \$?"
	test "rm -rf /tmp/a ; echo aaa >/tmp >/tmp >/tmp/a ; ls -l /tmp/a ; echo \$?"
	test "rm -rf /tmp/a ; echo aaa >/tmp/a /tmp >/tmp ; ls -l /tmp/a ; echo \$?"
	test "rm -rf /tmp/a /tmp/b /tmp/c ; echo aaa >> /tmp/a >> /tmp/b >> /tmp/c ; ls -l /tmp/a /tmp/b /tmp/c ;  cat /tmp/a /tmp/b /tmp/c ; echo \$?"
	test "rm -rf /tmp/a /tmp/b /tmp/c ; touch /tmp/a /tmp/b /tmp/c ; chmod 000 /tmp/a /tmp/b /tmp/c ; echo aaa > /tmp/a > /tmp/b > /tmp/c ; ls -l /tmp/a /tmp/b /tmp/c ;  cat /tmp/a /tmp/b /tmp/c ; echo \$?"
	test "echo >/dev ; echo \$?"
 }

test_correction_pipes() {

	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "ls | sort | cut -b 1-6"
	test "echo aaa | cat"
	test "ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls"
	test "ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |aaaaaaaaaaaa |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls |ls"
	test "true | false | true"
	test "false | false | true"
	test "cat | cat | cat | grep 42"
	test "cat | cat | cat"
	test "cat | cat | cat |"
	test "| ls | sort"
	test "rm -rf /tmp/a /tmp/b ; touch /tmp/a | ls >/dev | touch /tmp/b ; ls -l /tmp/a /tmp/b"
	test "rm -rf /tmp/a /tmp/b /tmp/c ; echo aaa > /tmp/a | echo bbb >/tmp/b | cal > /tmp/c ; ls -l /tmp/a /tmp/b /tmp/c ; cat /tmp/a /tmp/b /tmp/c"
	test "ls | grep XXXXXXX | sort -r | cut -b 1-1 | cat"
	test "ls | grep | sort -r | cut -b 1-1 | cat"
	test "ls | grep e | sort -r | aaaaaaa | cat"

	SORT_RESULT=TRUE
	test "ls >/tmp >/tmp/a | fake2"
	test "fake1 ; ls >/tmp >/tmp/a | fake2"
	test "ls >/tmp >/tmp/a | fake2 ; echo \$?"
	test "ls >/tmp >/tmp/a | fake2 ; echo \$?"
	test "ls >/tmp >/tmp/a | fakecommande ; echo \$?"
	test "ls >/tmp/a >/tmp | fakecommande ; echo \$?"
	test "ls <./aaaaaaaaa >/tmp/a | fakecommande ; echo \$?"
	test "ls >/tmp <./aaaaaaaaa | fakecommande ; echo \$?"
	test "touch /tmp/xxx ; chmod 000 /tmp/xxx ; ls </tmp/xxx >/tmp/a | fakecommande ; echo \$? ; rm -rf /tmp/xxx"
	test "touch /tmp/xxx ; chmod 000 /tmp/xxx ; ls >/tmp/xxx | fakecommande ; echo \$? ; rm -rf /tmp/xxx"

	test "echo 000001 ; sleep 0.4 ; fake3"
	test "fake1 | fake2 | fake3 | sleep 0.4 | echo 00002 | fake4"

	test "test fake1 | fake2 | fake3 | fake4 "
	test "fake1 | fake2 | fake3 | fake4 | echo _____________________"
	test "fake1 | fake2 | fake3 | fake4 | echo __ >/tmp ;echo ; echo"
	test "echo __ >/tmp | fake1 | fake2 | fake3 | fake4 ; echo ; echo"
	test "echo 00001 | sleep 0.4 | fake1 | sleep 0.4 | echo 00002"
	unset SORT_RESULT
}

test_correction_AND_OR () {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "true && echo aaa"
	test "false && echo aaa"
	test "true || echo aaa"
	test "false || echo aaa"
	test "aaaaaaaa && echo aaa"
	test "aaaaaaaa || echo aaa"
	test "cd /xxxxxxxxxx && echo aaa"
	test "cd /xxxxxxxxxx || echo aaa"
	test "true || true && true && echo aaa"
	test "true && true && true || echo aaa"
	test "false && false || true && echo aaa"
	test "false && false || false && echo aaa"

}

test_signal() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "fail_bin/buserror;"
	test "fail_bin/abort"
	test "fail_bin/segfault"
}

test_syntax() {
	print_separator '█'
	echo -e "\n\n\e[34m \e[1m 🌈 [$FUNCNAME]\n \e[0m"

	test "ls >"
	test "1"
	test "1 2 3 4 5 6 7 8 9"
	test "-1 -2"
	test "\\\n"
	test ";"
	test "|"
	test ";|"
	test ">>"
	test "<"
	test ">"
	test ";>>|><"
}

main () {
	true > /tmp/test_ko
	true > /tmp/minishell_sumup
	true > /tmp/bash_sumup
	unset SORT_RESULT
	export EXTRA_FLAGS=
	export TEST_NB=0
	export TEST_FAILED_NB=0
	export EXTRA_ENV='TERM=xterm-256color'

	if [[ -n "$1" ]]
	then
		test "$1"
		echo -e "\n [RESULT]\n"
		print_diff_full
	else

	#  test_random
	#  test_bonus
	#  test_executor
	#  test_failed
	#  test_signal
	#  test_syntax

	# test_correction_arg
	# test_correction_echo
	# test_correction_exit
	# test_correction_exec
	# test_correction_return
	# test_correction_semicolons
	# test_correction_baskslashs
	# test_correction_env

	# test_correction_export_identifier
	# test_correction_export_identifier_mix_valid
	# test_correction_export
	# test_correction_unset_identifier
	# test_correction_unset_identifier_mix_valid
	# test_correction_unset

	# test_correction_exp
	# test_correction_cd
	# test_correction_pwd
	# test_correction_PATH
	# test_correction_simple_quotes
	# test_correction_redirect
	# test_correction_pipes
	# test_correction_AND_OR

	fi

	print_separator '█'
	if [[ $TEST_FAILED_NB -gt 0 ]]
	then
		echo -e "\n\n\e[31m \e[1m\t\t[ ❌  FAILED TEST : $TEST_FAILED_NB / $TEST_NB ] \n\e[0m"
		echo -e "$(date): [ FAILED TEST :   $TEST_FAILED_NB / $TEST_NB ] ❌ " >> /tmp/history.log
		cat /tmp/test_ko
	else
		echo -e "\n\n\e[32m \e[1m\t\t[ ✅  SUCCESS TEST : $TEST_NB / $TEST_NB ] \n\e[0m"
		echo -e "$(date): [ FAILED TEST :   $TEST_FAILED_NB / $TEST_NB ] ✅ " >> /tmp/history.log
	fi
	echo -e "\n\n		---- HISTORY 10 last run ---\n"
	cat /tmp/history.log | tail --lines=10

}

main "$1"
