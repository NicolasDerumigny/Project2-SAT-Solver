#ifndef FORMULE_H
#define FORMULE_H
#include <map>
#include <algorithm>
#include "litt.h"
#include "global_variable_extern.h"
using namespace std;


class clause;
class var;

class formule{
public:
    void set_formule(int varid, bool neg);
    void set_formule_tseitin(bool neg);
    void set_formule_var(var* var, bool neg);
    void print();
    void free_formule();
    void preprocessing();
    string proof_str();

    clause* f_ClauseSatisfied=nullptr;
    clause* f_ClauseUnsatisfied=nullptr;
    clause* l_ClauseSatisfied=nullptr;
    clause* l_ClauseUnsatisfied=nullptr;

//    map<unsigned int,clause*> mClauseSatisfied;
//    map<unsigned int,clause*> mClauseUnsatisfied;
    void merge(formule* formule2);
    //merge this and formule 2 and place it into this
};

#endif // FORMULE_H
