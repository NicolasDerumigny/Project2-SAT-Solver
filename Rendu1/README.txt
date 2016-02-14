SOLVEUR SAT DPLL -- Projet 2 R.Staub et N.Derumigny
(S'applique uniquement aux formules sous forme normales conjonctives)



Pour compiler, lancer
make

Pour executer le programme, lancer
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


Ce programme évalue le fichier à l'aide de bison/flex : et construit au fur et à mesure :
- Des objets de type formules, contenants des objets de types clauses séparées en satisfaites et insatisfaites
- Des objets de type clauses, contenant des objets de types littéraux séparées en vivants et morts
- Des objets de type littéraux, contenant un pointeur vers un objet de type variable et un bouléen désignant si la variable est niée ou non
- Des objets de type variables, contenant leur numéro et leur affectation (vrai/faux/non attribuée)

Une fonction d'affichage de la sortie bison est implémentée au moyen da la méthode to_string()
Une fonction d'affichage de l'objet de type formule ainsi crée est aussi implémenté par la méthode print()

