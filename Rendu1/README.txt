SOLVEUR SAT DPLL -- Projet 2 R.Staub et N.Derumigny
(S'applique uniquement aux formules sous forme normales conjonctives)



Pour compiler, lancer
make

Pour exécuter le programme, lancer
./resol FichierSource.cnf

Le fichier source doit être de la forme :

p cnf X Y
   où X est l'indice maximal des variables utilisées, Y le nombre de clauses

suivis de Y lignes de la forme :
X1 X2 ... Xn 0

Ou le Xi sont soit le numéro de la variable, soit -Yi où Yi est le numéro de la variable, symbolisant non(Yi).

Chaque ligne représente une clause (disjonction de ses variable).
La formule finale est la conjonction de toutes ses clauses.
0 marque la fin de la ligne est ne doit pas être utilisé comme variable.

-------------

Générateur aléatoire de formules :

./Rendu1/Examples/generate_cnf.sh

usage : generate_cnf.sh [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses]

L'ajout aléatoire de commentaires n'a pas été pris en compte (par manque de générateur aléatoire de phrases installé par défaut sur les machines libre-service)

-------------

Description :

Ce programme évalue le fichier à l'aide de bison/flex : et construit au fur et à mesure (par appels à la méthode Expr::eval) :
- Des objets de type formules, contenants des objets de types clauses séparées en satisfaites et insatisfaites
- Des objets de type clauses, contenant des objets de types littéraux séparées en vivants et morts
- Des objets de type littéraux, contenant un pointeur vers un objet de type variable et un bouléen désignant si la variable est niée ou non
- Des objets de type variables, contenant leur numéro et leur affectation (vrai/faux/non attribuée)

Une fonction d'affichage de la sortie bison est implémentée au moyen da la méthode to_string()
Une fonction d'affichage de l'objet de type formule ainsi créée est aussi implémenté par la méthode print()



-------------

Améliorations :

Mettre à jour Bison/Flex (warning lié au passage au C++11 dans expr.yy.c lors de la compilation exigeante avec l'option -Wall).

On ne prend pas en compte les clauses déclarées avant la ligne d'en-tête.

On gère mal des commentaires ne commençant pas par 'c'.

Pré-traiter les entrées.

Temps de construction de la formule beaucoup trop long (comparé à minisat).

-------------

Répartition du travail :

La grande majorité du code a été écrit ensemble. Les exceptions étant définies ci-après.

Nicolas D. :
- file_open.cpp
- backtrack.cpp

Ruben S. :
- deduce.cpp
- decide.cpp
- generate_cnf.sh
