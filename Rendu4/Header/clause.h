#ifndef CLAUSE_H
#define CLAUSE_H
#include <map>
#include <iostream>
#include "global_variable_extern.h"


class litt;

class clause{
public:
    void set_clause(litt* litt_entry);
    void print();
    bool isSatisfied();
    bool satisfied = false;
    void merge(clause* cl);
    int nbLittAlive();
    float score = 0;
    bool isLearned = false;
    litt* getUniqueLittAlive();
    bool existsWatchedNonAlive();
    clause* copy();
    void free_clause();
    std::string proof_str(bool complete);

    clause* next_clause = nullptr;
    clause* prev_clause = nullptr;

    litt* f_ElementAlive=nullptr;//premier de la liste chainée
    litt* l_ElementAlive=nullptr;//dernier de la liste chainée
    litt* f_ElementDead=nullptr;//premier de la liste chainée
    litt* l_ElementDead=nullptr;//dernier de la liste chainée

    litt* w_litt_1=nullptr;
    litt* w_litt_2=nullptr;
};

void removeClause(clause **first_cl,clause **last_cl,clause *cur_cl);
void appendClause(clause **first_cl, clause **last_cl,clause *cur_cl);
litt* getFirstTrue(litt* entry);
#endif // CLAUSE_H
