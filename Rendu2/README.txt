SOLVEUR SAT DPLL -- Projet 2 R.Staub et N.Derumigny



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

usage : generate_cnf.sh [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses] ([non-rand])

L'ajout aléatoire de commentaires n'a pas été pris en compte (par manque de générateur aléatoire de phrases installé par défaut sur les machines libre-service).
L'option non-rand permet de fixer le nombre de littéraux par clause, utile pour les tests de performance.

-------------

Description :

Ce programme évalue le fichier à l'aide de bison/flex : et construit au fur et à mesure (par appels à la méthode Expr::eval) :
- Des objets de type formules, contenants des objets de types clauses séparées en satisfaites et insatisfaites
- Des objets de type clauses, contenant des objets de types littéraux séparées en vivants et morts, et un id unique par clause
- Des objets de type littéraux, contenant un pointeur vers un objet de type variable et un bouléen désignant si la variable est niée ou non
- Des objets de type variables, contenant leur numéro et leur affectation (vrai/faux/non attribuée), ainsi qu'un vector de pointeur vers les clauses le contenant
- Des objets de type assignation, contenant une variable et si sa valeur est un pari ou non

Une fonction d'affichage de la sortie bison est implémentée au moyen da la méthode to_string()
Une fonction d'affichage de l'objet de type formule ainsi créée est aussi implémenté par la méthode print()

Trois variables globales sont déclarées dans global_variable : 
- instance, la formule entrée (type formule)
- v_var, le vecteur des pointeur de toutes les variables utilisées (ou nullptr si elles ne sont pas présentes dans la formule)
- assignations, le vectors des pointeurs vers paris effectués

L'ensemble des fonctions des parse, de gestion de l'annalyse syntaxique et de formattage du texte entré est dans file_open.cpp.

-------------

Améliorations :

On ne prend pas en compte les clauses déclarées avant la ligne d'en-tête.

On gère mal des commentaires ne commençant pas par 'c'.

Pré-traiter les entrées. (en particulier, gérer la mise à jour de clauseInto des clauses supprimées, voire gérer le linkage (clauseInto) avec le prétraitement). Majoritairement traité !

Tseitin (gérer les => via non(B et non A), ... ).

-------------

Répartition du travail :

La grande majorité du code a été écrit ensemble. Les exceptions étant définies ci-après.

Nicolas D. :
- file_open.cpp
- backtrack.cpp
- Tseitin

Ruben S. :
- deduce.cpp
- decide.cpp
- generate_cnf.sh
- Heuristiques
