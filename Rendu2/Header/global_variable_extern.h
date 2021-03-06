#ifndef GLOBAL_VARIABLE_EXTERN_H
#define GLOBAL_VARIABLE_EXTERN_H
#include <ctime>
#include <vector>
#include "var.h"
using namespace std;

class var;
class formule;
class assignation;

extern vector<assignation*> assignations;
//création du vector qui contient les assignations réalisées

extern vector<var*> v_var;
extern int nbr_var;
extern vector<var*> v_var_tseitin;
//création du vector qui contiendra tous les pointeurs vers les variables

extern formule * instance;
//une formule pour les gouverner tous
extern clock_t checkpoint;


extern int heuristic;
extern bool isTseitin;
extern bool verbose;
extern bool verbose2;
extern bool timePerf;
#endif // GLOBAL_VARIABLE_EXTERN_H
