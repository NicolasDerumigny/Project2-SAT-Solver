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


