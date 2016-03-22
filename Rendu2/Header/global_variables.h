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
//création du vector qui contiendra tous les pointeurs vers les variables

formule *instance;
//une formule pour les gouverner tous
clock_t checkpoint = clock();

bool isTseitin=false;
bool verbose=false;
bool timePerf=false;
//valeur de base des arguments

#endif // GLOBAL_VARIABLES_H
