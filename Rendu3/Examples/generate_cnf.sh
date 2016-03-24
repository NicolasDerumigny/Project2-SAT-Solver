#!/bin/bash

[[ $# -lt 3 ]] && echo "Nombre d'arguments insuffisant : generate_cnf.sh [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses] ([non-rand])" && exit

nb_var=$1
nb_clauses=$2
nb_litt=$3
if [ "$4" == "non-rand" ]
then
	echo "p cnf $nb_var $nb_clauses"
	for i in $(seq 1 $nb_clauses)
	do
		for j in $(seq 1 $nb_litt)
		do
			[[ $RANDOM%2 -eq 1 ]] && printf "-"
			printf "$[($RANDOM%$nb_var)+1] "
		done
		printf "0\n"
	done
	exit
else
	echo "p cnf $nb_var $nb_clauses"
	for i in $(seq 1 $nb_clauses)
	do
		for j in $(seq 1 $[($RANDOM%$nb_litt)+1])
		do
			[[ $RANDOM%2 -eq 1 ]] && printf "-"
			printf "$[($RANDOM%$nb_var)+1] "
		done
		printf "0\n"
	done
	exit
fi
