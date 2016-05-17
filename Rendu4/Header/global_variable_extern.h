#ifndef GLOBAL_VARIABLE_EXTERN_H
#define GLOBAL_VARIABLE_EXTERN_H
#ifdef __clang__
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#include <ctime>
#include <vector>
#include <fstream>
#include <map>
#include <tuple>
#include "var.h"


class var;
class formule;
class assignation;

extern std::vector<assignation*> assignations;
extern int level_cur;
//création du vector qui contient les assignations réalisées

extern std::vector<var*> v_var;
extern unsigned long nbr_var;
extern std::vector<var*> v_var_tseitin;
extern std::vector<int> bets;
//création du vector qui contiendra tous les pointeurs vers les variables

extern formule * instance;
//une formule pour les gouverner tous

extern clock_t checkpoint;
extern unsigned long index_t;

extern int heuristic;
extern int clHeuristic;
extern bool isTseitin;
extern bool verbose;
extern bool verbose2;
extern bool timePerf;
extern bool interactive;
extern bool verboseParser;
extern bool clLearning;
extern bool wl;
extern bool satModEq;
extern int nb_threads;
extern char path[1024];
extern bool proof;
extern std::fstream prooftree_fs;
extern std::map< std::tuple<unsigned long, unsigned long, bool>,unsigned long > eqToVar;
extern std::map<unsigned long, std::tuple<unsigned long, unsigned long, bool> > varToEq;
#endif // GLOBAL_VARIABLE_EXTERN_H
