#!/bin/bash

[[ $# -lt 7 ]] && echo -e 'Usage : "[lien vers generate_cnf.sh | lien vers fichiers de tests]" "[lien vers solveur]" [nombre de tests] [nombre de variables] [nombre de clauses] [nombre de littéraux par clauses] ["options" pour le solveur]\n\nAttention : les ratios doivent être entiers\nLes options pour le solveur devront être séparés par un espace et avec guillemets englobants pour chaque groupe. Example : "-cl" "" "-rand" "-dlis -tseitin" est acceptable\nAttention : Les liens vers les fichiers de test doivent être entre guillemets, séparés par des espaces. Ainsi, les chemins contenant des espaces (même échappés par un \\) ne peuvent pas être utilisés.\nAttention : le fichier temporaire /tmp/test.cnf sera créé/remplacé.\nAstuce : si on fournit les tests (ex : "/tmp/*.cnf"), alors il est conseillé de renseigner le nombre de fichiers fournis dans le nombre de tests (Si plus est indiqué, des tests sont refaits. Si moins, tous les tests ne sont pas effectués). De même, les champs [nombre de variables/clauses/littéraux] ne seront pas considérés (mais restent obligatoires)' && exit

generator_path="$1"
nb_files=$(ls -l $generator_path|wc -l)
shift
solver_path="$1"
shift
nb_tests="$1"
shift
nb_var="$1"
shift
nb_clauses="$1"
shift
nb_litt="$1"
shift
nb_opt=0
while [ $# -gt 0 ]
do
	options[$nb_opt]="$1"
	nb_opt=$[$nb_opt+1]
	shift
done
#Initialisation des compteurs
for j in $(seq 0 $[$nb_opt-1])
do
	l_begin[$j]=0
	l_check[$j]=0
	l_parse[$j]=0
	l_create[$j]=0
	l_end[$j]=0
done


rm /tmp/results.dat
for i in $(seq 1 $nb_tests)
do
	[[ "$generator_path" =~ "generate_cnf.sh" ]] && "$generator_path" $nb_var $nb_clauses $nb_litt non-rand > /tmp/test.cnf || cp $(ls $generator_path|tr '\n' ' '|cut -d\  -f$[$i%($nb_files+1)]) /tmp/test.cnf
	for j in $(seq 0 $[$nb_opt-1])
	do
		echo "${options[$j]}"
		results="$("$solver_path" /tmp/test.cnf -time ${options[$j]} 3>&1 1>&2 2>&3-)"
		l_begin[$j]="$(echo "${l_begin[$j]} + $(echo "$results"|grep "^begin: [0-9.]* s$"|sed 's/^begin: \([0-9.]\+\) s$/\1/g')"|bc -l)"
		l_check[$j]="$(echo "${l_check[$j]} + $(echo "$results"|grep "^check: [0-9.]* s$"|sed 's/^check: \([0-9.]\+\) s$/\1/g')"|bc -l)"
		l_parse[$j]="$(echo "${l_parse[$j]} + $(echo "$results"|grep "^parse: [0-9.]* s$"|sed 's/^parse: \([0-9.]\+\) s$/\1/g')"|bc -l)"
		l_create[$j]="$(echo "${l_create[$j]} + $(echo "$results"|grep "^create: [0-9.]* s$"|sed 's/^create: \([0-9.]\+\) s$/\1/g')"|bc -l)"
		l_end[$j]="$(echo "${l_end[$j]} + $(echo "$results"|grep "^end: [0-9.]* s$"|sed 's/^end: \([0-9.]\+\) s$/\1/g')"|bc -l)"
	done
	#echo "$results, ${l_begin[*]}, ${l_check[*]}, ${l_parse[*]}, ${l_create[*]}, ${l_end[*]}"
done #2>> /tmp/results.dat

for j in $(seq 0 $[$nb_opt-1])
do
	l_begin[$j]="$(echo "${l_begin[$j]}/$nb_tests"|bc -l)"
	l_check[$j]="$(echo "${l_check[$j]}/$nb_tests"|bc -l)"
	l_parse[$j]="$(echo "${l_parse[$j]}/$nb_tests"|bc -l)"
	l_create[$j]="$(echo "${l_create[$j]}/$nb_tests"|bc -l)"
	l_end[$j]="$(echo "${l_end[$j]}/$nb_tests"|bc -l)"
done

echo -e "option\\step\tbegin\tcheck\tparse\tcreate\tend" > /tmp/results.dat
for j in $(seq 0 $[$nb_opt-1])
do
	echo -e "${options[$j]}\t${l_begin[$j]}\t$(echo "${l_check[$j]} - ${l_begin[$j]}"|bc -l)\t$(echo "${l_parse[$j]} - ${l_check[$j]}"|bc -l)\t$(echo "${l_create[$j]} - ${l_parse[$j]}"|bc -l)\t$(echo "${l_end[$j]} - ${l_create[$j]}"|bc -l)" >> /tmp/results.dat
done

gnuplot -p -e "set style data histograms;set style histogram rowstacked;set title 'data : $nb_tests $nb_var $nb_clauses $nb_litt';set xlabel 'Options provided';set ylabel 'Time elapsed (sec)';set style fill solid 0.8 border -1;set boxwidth 0.3;set datafile separator '\t';plot for [col=2:6] '/tmp/results.dat' using col:xtic(1) t columnhead(col)"

#i=1
#rm /tmp/results2.dat
#cat /tmp/results.dat|tr '\n' '\t'|sed -e 's/begin/\n/g' -e 's/[^0-9.\t\n]//g'|tail -n +2|echo "$(cat)"|while read line
#do 
#	echo -e "$[$nb_var_max*$i]\t$line" >> /tmp/results2.dat
#	i=$[$i+1]
#done

#gnuplot -p -e "set style data histograms;set style histogram rowstacked;set xlabel 'Number of variables';set ylabel 'Time elapsed (sec)';plot '/tmp/results2.dat' using 2 t columnhead(1), '' using 3 t columnhead(2), '' using 4:xtic(1) t columnhead(3)"
#plot '/tmp/results2.dat' u 1:2 title 'begin' w l, '/tmp/results2.dat' u 1:3 title 'check' w l, '/tmp/results2.dat' u 1:4 title 'parse' w l, '/tmp/results2.dat' u 1:5 title 'create' w l, '/tmp/results2.dat' u 1:6 title 'end' w l"

exit
