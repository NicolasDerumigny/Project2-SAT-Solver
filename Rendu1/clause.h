#ifndef CLAUSE_H
#define CLAUSE_H
#include "litt.h"
#include <map>
using namespace std;

class clause{
public:
    void set_clause(litt* litt_entry);
    void print();

    map<unsigned int,litt*> mElementAlive;
    map<unsigned int,litt*> mElementDead;
    bool isSatisfied();
    void merge(clause* cl);
};

#endif // CLAUSE_H
