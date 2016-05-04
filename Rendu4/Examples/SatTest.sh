#!/bin/bash

#function echo_help_create {
#	echo 'usage : SatTest.sh create "[lien vers generate_cnf.sh]" "[répertoire des tests]" [nombre de tests] [cnf | tseitin | all] [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses]'
#}

#function echo_help_test {
#	echo 'usage : SatTest.sh test "[executable(s)]" "[repertoire des tests]" "[options pour le solveur]"'
#}

#function echo_help_clean {
#	echo 'usage : SatTest.sh clean "[repertoire des tests]"'
#}

#function echo_help {
#	echo_help_create
#	echo 'puis,'
#	echo_help_test
#	echo 'enfin,'
#	echo_help_clean
#	echo -e '\nAttention : les "" doivent être respectés comme indiqué ci-dessus'
#	echo 'Attention : les chemins renseignés doivent exister'
#	echo -e '\nUtilisation : commencer par créer des tests dans un dossier ("." pour le répertoire courant, "/tmp/" est conseillé), puis tester le(s) executable(s), (et enfin nettoyer les tests créés)'
#}

#case "$1" in

#"create")
#	[[ $# -ne 8 ]] && echo_help_create && exit
#	[[ "$5" != "cnf" ]] && [[ "$5" != "tseitin" ]] && [[ "$5" != "all" ]] && echo_help_create && exit
#	formats=( "cnf" "for" )
#	[[ "$5" == "cnf" ]] && format=${formats[0]}
#	[[ "$5" == "for" ]] && format=${formats[1]}
#	for i in $(seq 1 $4)
#	do
#		[[ "$5" == "all" ]] && format=${formats[$RANDOM%2]}
#		"$2" "$6" "$7" "$8" "$([[ $format == 'for' ]] && echo 'tseitin')" > "$3"/test_sat_autogen_"$i"."$format"
#	done
#	;;
#"clean")
#	[[ $# -ne 2 ]] && echo_help_clean && exit
#	rm "$2"/test_sat_autogen_*.*
#	;;
#"test")
#	[[ $# -ne 4 ]] && echo_help_test && exit
#	echo TODO
#	;;
#*)
#	echo_help
#	;;
#esac

#exit

[[ $# -lt 5 ]] && echo -e 'Usage : "[lien vers generate_cnf.sh]" "[lien vers solveur]" [nombre de variables maximum] [ratio : nb clauses / nb variables] [ratio : nb variables / nb litt par clause] ("[options pour le solveur]")\n\nAttention : les ratios doivent être entiers' && exit

generator_path="$1"
shift
solver_path="$1"
shift
nb_var_max=$[$1/10]
shift
ratio_cl="$1"
shift
ratio_li="$1"
shift

for i in {1..10};do "$generator_path" $[$nb_var_max*$i] $[$nb_var_max*$i*$ratio_cl] $[($nb_var_max*$i)/$ratio_li] non-rand > /tmp/test.cnf;"$solver_path" /tmp/test.cnf $* -time;done 2> /tmp/results.dat;i=1;rm /tmp/results2.dat;cat /tmp/results.dat|tr '\n' '\t'|sed -e 's/begin/\n/g' -e 's/[^0-9.\t\n]//g'|tail -n +2|echo "$(cat)"|while read line;do echo -e "$[$nb_var_max*i]\t$line" >> /tmp/results2.dat;i=$[$i+1];done;gnuplot -p -e "set xlabel 'Number of variables';set ylabel 'Time elapsed (sec)';plot '/tmp/results2.dat' u 1:2 title 'begin' w l, '/tmp/results2.dat' u 1:3 title 'check' w l, '/tmp/results2.dat' u 1:4 title 'parse' w l, '/tmp/results2.dat' u 1:5 title 'create' w l, '/tmp/results2.dat' u 1:6 title 'end' w l"

exit
