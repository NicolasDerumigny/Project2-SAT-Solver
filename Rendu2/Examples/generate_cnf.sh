#!/bin/bash

[[ $# -lt 3 ]] && echo "Nombre d'arguments insuffisant : generate_cnf.sh [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses]" && exit

nb_var=$1
nb_clauses=$2
nb_litt=$3

echo "p cnf $nb_var $nb_clauses"
for i in $(seq 1 $nb_clauses)
do
	for j in $(seq 1 $nb_litt) #$[($RANDOM*$nb_litt)/(2**15)+1])
	do
		[[ $RANDOM -gt $[2**14] ]] && printf "-"
		printf "$[($RANDOM*$nb_var)/(2**15)+1] "
	done
	printf "0\n"
done
exit
