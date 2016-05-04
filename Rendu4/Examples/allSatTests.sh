#!/bin/bash

function echo_help {
	echo 'Usage : allSatTest.sh "[lien vers le solveur]" "[lien vers generate_cnf]" [nombre de tests] [nombre de variables] [nombre de clauses] [nombre de littéraux par clauses] ("save") (-v)'
	echo "L'option \"save\" permet de conserver les tests générés d'une session à l'autre"
#	echo "Le nombre de tests réel est doublé du fait que l'on génère aussi avec tseitin" 
}

[[ $# -lt 6 ]] && echo_help && exit

solver_path="$1"
generator_path="$2"
nb_tests="$3"
nb_var="$4"
nb_clauses="$5"
nb_litt="$6"
add_option=" $7 $8 "
tests_found="$(ls|grep "rstaub_allSatTests_autogen_*.*"|wc -l)"

if ([ $tests_found != 0 ]) # && [ $[$tests_found%2] == 0 ])
then
	echo "$tests_found tests retrouvés avec succès. Évaluation des performances sur ces tests."
else #genere tests normaux et tseitin dans le dossier courant
	for i in $(seq 1 $nb_tests)
	do
		"$generator_path" $nb_var $nb_clauses $nb_litt non-rand > "rstaub_allSatTests_autogen_$i.cnf"
#		"$generator_path" $nb_var $nb_clauses $nb_litt tseitin non-rand > "rstaub_allSatTests_autogen_$i.for"
		[[ "$add_option" =~ " -v " ]] && echo "$i tests générés sur $nb_tests"
	done
fi

#On évalue :
#temps
#correction (sauf si tseitin)
#Avec :
#heuristiques
#tseitin
#littéraux surveillés
#apprentissage de clauses

for heur_opt in "" "-rand" "-dlis" "-moms" #"-cl -vsids" "-cl -forget"
do
	for wl_opt in "" #"-wl" watched litterals are still work in progress...
	do
		for cl_opt in "" "-cl"
		do
			echo "Test options : $heur_opt $wl_opt $cl_opt"
			tseitin_opt=""
			true_results=0
			time_total=0
			tested=0
			for fichier in rstaub_allSatTests_autogen_*.cnf
			do
				result="$(TIMEFORMAT="real %E";(time -p "$1" $fichier $heur_opt $wl_opt $cl_opt) 2>&1)"
				tested=$[$tested+1]
				time_total=$(echo "$time_total+$(echo "$result"|grep "^real\ [0-9]*.[0-9]*$"|cut -d\  -f2)"|bc -l)
#				satisfiability=$(echo "$result"|grep -e "^s SATISFIABLE$" -e "^s UNSATISFIABLE$"|cut -d\  -f2)
#				echo "$(minisat $fichier 2> /dev/null |tail -1) == $satisfiability"
#				echo $fichier
#				[[ "$(minisat $fichier 2> /dev/null |tail -1)" == "$satisfiability" ]] && true_results=$[$true_results+1]
				[[ "$add_option" =~ " -v " ]] && [[ $[$tested%($nb_tests/10)] == 0 ]] && echo "$[((100*$tested)/$nb_tests)]% testés : temps=$(echo "$time_total/$tested"|bc -l)" #correction=$(echo "100*$true_results/$tested"|bc)"
			done
			echo -e "Avec options : $heur_opt $wl_opt $cl_opt \t-> temps moyen = $(echo "$time_total/$tested"|bc -l)"
#			tseitin_opt="-tseitin"
#			true_results=0
#			time_total=0
#			tested=0
#			for fichier in rstaub_allSatTests_autogen_*."for"
#			do
#				result="$(TIMEFORMAT="real %E";(time "$1" $fichier $heur_opt $wl_opt $cl_opt) 2>&1)"
#				tested=$[$tested+1]
#				time_total=$(echo "$time_total+$(echo "$result"|grep "^real\ [0-9]*.[0-9]*$"|cut -d\  -f2)"|bc -l)
#				[[ "$add_option" =~ " -v " ]] && [[ $[$tested%($nb_tests/20)] == 0 ]] && echo "$[((100*$tested)/$nb_tests)]% testés : temps=$(echo "$time_total/$tested"|bc)"
#			done
#			echo "Avec options : $heur_opt $wl_opt $cl_opt -tseitin -> temps moyen = $(echo "$time_total/$tested"|bc -l)"
		done
	done
done
[[ "$add_option" =~ " save " ]] && exit || rm rstaub_allSatTests_autogen_*.*
