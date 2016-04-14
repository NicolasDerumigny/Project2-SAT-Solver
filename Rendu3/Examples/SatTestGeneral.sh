!/bin/bash

[[ $# -lt 5 ]] && echo -e 'Usage : "[lien vers generate_cnf.sh]" "[lien vers solveur]" [nombre de variables maximum] [ratio : nb clauses / nb variables] [ratio : nb variables / nb litt par clause] ("[options pour le solveur]")\n\nAttention : les ratios doivent être entiers' && exit

generator_path="$1"
shift
solver_path="$1"
shift
nb_var_max=$["$1"/10]
shift
ratio_cl="$1"
shift
ratio_li="$1"
shift

for i in {1..10};do "$generator_path" $[$nb_var_max*$i] $[$nb_var_max*$i*$ratio_cl] $[($nb_var_max*$i)/$ratio_li] non-rand > /tmp/test.cnf;"$solver_path" /tmp/test.cnf $* -time;done 2> /tmp/results.dat;i=1;rm /tmp/results2.dat;cat /tmp/results.dat|tr '\n' '\t'|sed -e 's/begin/\n/g' -e 's/[^0-9.\t\n]//g'|tail -n +2|echo "$(cat)"|while read line;do echo -e "$[$nb_var_max*i]\t$line" >> /tmp/results2.dat;i=$[$i+1];done;gnuplot -p -e "set xlabel 'Number of variables';set ylabel 'Time elapsed (sec)';plot '/tmp/results2.dat' u 1:2 title 'begin' w l, '/tmp/results2.dat' u 1:3 title 'check' w l, '/tmp/results2.dat' u 1:4 title 'parse' w l, '/tmp/results2.dat' u 1:5 title 'create' w l, '/tmp/results2.dat' u 1:6 title 'end' w l"

exit
