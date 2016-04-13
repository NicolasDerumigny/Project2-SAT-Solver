SOLVEUR SAT DPLL -- Projet 2 R.Staub et N.Derumigny




Pour compiler, lancer
make


Pour nettoyer les fichiers de compilation, lancer 
make clean


Pour exécuter le programme, lancer
./resol FichierSource[.cnf | .for] [-v | -vv] [-time] [-moms | -rand | -dlis] [-tseitin] [-cl] [-wl]

Le fichier source doit être de la forme :

× Si l'extension est .cnf :

	p cnf X Y
	où X est l'indice maximal des variables utilisées, Y le nombre de clauses

	suivis de Y lignes de la forme :
	X1 X2 ... Xn 0

	Ou le Xi sont soit le numéro de la variable, soit -Yi où Yi est le numéro de la variable, symbolisant non(Yi).

	Chaque ligne représente une clause (disjonction de ses variable).
	La formule finale est la conjonction de toutes ses clauses.
	0 marque la fin de la ligne est ne doit pas être utilisé comme variable.


× Si l'extension est .for (utilisation avec -tseitin) :
	Eventuellement un ligne d'en-tête (ou pas)
	Des lignes de la forme :
	X1 T1 X2 T2 .... Tn Xn

	où :
		les Xi est soit un numero de variable, soit -Yi
		les Ti sont 'X' (ou exclusif), => (implique), <=> (equivaut), /\ (et), \/ (ou) eventuellement suivit de ~ niant la sous-formule suivante

	Le retour chariot est interprété comme un "et", un espace comme un "ou". Les parenthésages corrects sont bien gérés, cependant les mauvais parenthésages ne sont pas détectés (plantage du parseur).


------------------------------------

Générateur aléatoire de formules :

./Rendu1/Examples/generate_cnf.sh

usage : generate_cnf.sh [nombre de variables] [nombre de clauses] [nombre max de littéraux par clauses] ([non-rand | tseitin])

L'ajout aléatoire de commentaires n'a pas été pris en compte (par manque de générateur aléatoire de phrases installé par défaut sur les machines libre-service).
L'option non-rand permet de fixer le nombre de littéraux par clause, utile pour les tests de performance.
L'option tseitin permet de générer des tests pour tseitin (opérateurs choisis aléatoirement parmi : 'X' '\/' '/\' '=>' '<=>' '~' ' ').

------------------------------------

Description :

Ce programme évalue le fichier à l'aide de bison/flex : et construit au fur et à mesure (par appels à la méthode Expr::eval) :
- Des objets de type formules, contenants des objets de types clauses séparées en satisfaites et insatisfaites via des listes chainées
- Des objets de type clauses, contenant des objets de types littéraux séparées en vivants et morts via des listes chainées, et la clause suivante dans la liste
- Des objets de type littéraux, contenant un pointeur vers un objet de type variable et un bouléen désignant si la variable est niée ou non, et son suivant dans la liste
- Des objets de type variables, contenant leur numéro et leur affectation (vrai/faux/non attribuée), ainsi qu'un vector de pointeurs vers les clauses le contenant et un vector représentant les conflits (pointeur vers les variables qui l'impliquent)
- Des objets de type assignation, contenant une variable et si sa valeur est un pari ou non

Une fonction d'affichage de la sortie bison est implémentée au moyen da la méthode to_string()
Une fonction d'affichage de l'objet de type formule ainsi créée est aussi implémenté par la méthode print()

Trois variables globales sont déclarées dans global_variable : 
- instance, la formule entrée (type formule)
- v_var, le vecteur des pointeur de toutes les variables utilisées (ou nullptr si elles ne sont pas présentes dans la formule)
- assignations, le vectors des pointeurs vers paris effectués

En outre, chaque argument (-tseitin, -wl, etc) est lié à un bouléen global indiquant si l'option est activée ou non.

File_open.cpp contient l'ensemble des fonctions vérifiants la syntaxe, effectuant l'analyse syntaxique et libérant la memoire.

L'instance est pré-traitée juste avant de commencer l'algorithme DPLL (suppression des clauses tautologiques, initialisation des littéraux surveillés).

Avec les options -cl et -interac il est possible de voir le graphe des conflits en tapant 'g' lorsque le message "backtrack breakpoint" apparaît.
Pour cela, le fichier graph.dot est généré dans le dossier courant, puis il est converti en graph.ps, puis affiché avec evince.
Important : Linux est requis pour cette étape.
Attention : l'étape de convertion et d'affichage nécéssite d'avoir préalablement installé dot (testé avec la version 2.38.0 et 2.36.0 installée sur les salles libre service) et evince (installé aussi sur les salles libre service).
Pour vous les procurer, lancer dans un terminal : sudo apt-get update ; sudo apt-get install graphviz evince


-------------


Améliorations :

On ne prend pas en compte les clauses déclarées avant la ligne d'en-tête.

On gère mal des commentaires ne commençant pas par 'c'.

Créer un script de test général gérant aussi les performances. (en cours)

Commencer DPLL par les déductions possibles (implique de revoir backtrack...)

Rendre le code portable sur Mac et Windows (pour l'appel à dot et à evince, ainsi que bison)

Revoir l'algorithme utilisé car après un backtrack, on décide directement. deduce avant ?
while(){

deduce

if(conflits){
backtrack || conflictAnal
}else{
decide
}

}

-------------

Répartition du travail :

La grande majorité du code a été écrit ensemble. Les exceptions étant définies ci-après.

Nicolas D. :
- file_open.cpp
- backtrack.cpp
- Tseitin
- Littéraux surveillés
- Fonctions mineurs dans les apprentissages de clauses (copie)

Ruben S. :
- deduce.cpp
- decide.cpp
- generate_cnf.sh
- Heuristiques
- Graphe + Apprentissage de clauses (en cours)

-------------

Progression par rapport au rendu 2 :
- debogage complet de tseitin et des listes chainées
- littéraux surveillés, testé avec succès sur des formules à grand nombre de littéraux par clause
- apprentissage des clause : en cours

