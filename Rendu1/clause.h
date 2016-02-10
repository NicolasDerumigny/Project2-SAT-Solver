#include "litt.h"
#include <map>
#ifndef CLAUSE_H
#define CLAUSE_H
using namespace std;

class clause{
public:
    void set_clause(litt litt_entry);

    map<unsigned int,litt> mElementAlive;
    map<unsigned int,litt> mElementDead;
    bool isSatisfied();
    void merge(clause* cl);
};

#endif // CLAUSE_H
