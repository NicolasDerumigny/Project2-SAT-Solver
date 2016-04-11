#ifndef CLAUSE_H
#define CLAUSE_H
#include <map>
#include <iostream>
#include "global_variable_extern.h"
using namespace std;

class litt;

class clause{
public:
    void set_clause(litt* litt_entry);
    void print();
    bool isSatisfied();
    void merge(clause* cl);
    int nbLittAlive();
    litt* getUniqueLittAlive();
    void free_clause();

    clause* next_clause = nullptr;

    litt* f_ElementAlive=nullptr;//premier de la liste chainée
    litt* l_ElementAlive=nullptr;//dernier de la liste chainée
    litt* f_ElementDead=nullptr;//premier de la liste chainée
    litt* l_ElementDead=nullptr;//dernier de la liste chainée

    litt* w_litt_1=nullptr;
    litt* w_litt_2=nullptr;
};

void removeClause(clause **first_cl,clause **last_cl,clause *cur_cl,clause *prev_cl);
void appendClause(clause **first_cl, clause **last_cl,clause *cur_cl);

#endif // CLAUSE_H
