#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
#include <ctime>
#include <vector>
class assignation;
class var;
class formule;

vector<assignation*> assignations;
//création du vector qui contient les assignations réalisées

vector<var*> v_var;
vector<var*> v_var_tseitin;
int nbr_var=0;
//création du vector qui contiendra tous les pointeurs vers les variables

formule *instance;
//une formule pour les gouverner tous
clock_t checkpoint = clock();

bool isTseitin=false;
bool verbose=false;
bool verbose2=false;
bool timePerf=false;
bool interactive=false;
bool clLearning=false;
int heuristic=0;
char path[1024]="";
//valeur de base des arguments
//heuristic = 0 (basic), 1 (rand), 2 (moms), 3 (dlis)

#endif // GLOBAL_VARIABLES_H
