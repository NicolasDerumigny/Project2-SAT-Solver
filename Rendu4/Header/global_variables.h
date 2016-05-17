#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
#ifdef __clang__
#if __cplusplus == 201103L
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#endif
#include <ctime>
#include <vector>
#include <fstream>
#include <map>
#include <tuple>
#include "global_variable_extern.h"

class assignation;
class var;
class formule;

std::vector<assignation*> assignations;
int level_cur = 0;
//création du vector qui contient les assignations réalisées

std::vector<var*> v_var;
std::vector<var*> v_var_tseitin;
std::vector<int> bets;
unsigned long nbr_var=0;
//création du vector qui contiendra tous les pointeurs vers les variables

formule *instance;
//une formule pour les gouverner tous
clock_t checkpoint = clock();
unsigned long index_t;

bool isTseitin=false;
bool verbose=false;
bool verbose2=false;
bool timePerf=false;
bool interactive=false;
bool verboseParser=false;
bool clLearning=false;
bool wl=false;
bool satModEq=false;
int nb_threads=0;
int heuristic=0;
int clHeuristic=0;
char path[1024]="";
bool proof=false;
std::fstream prooftree_fs;
std::map<std::tuple<unsigned long, unsigned long, bool>,unsigned long > eqToVar;
std::map<unsigned long, std::tuple<unsigned long, unsigned long, bool> > varToEq;
//valeur de base des arguments
//heuristic = 0 (basic), 1 (rand), 2 (moms), 3 (dlis)

#endif // GLOBAL_VARIABLES_H
