#include <map>
#include "litt.h"
#ifndef CLAUSE_H
#define CLAUSE_H
using namespace std;

class clause{
public:
    map<unsigned int,litt> mElementAlive;
    map<unsigned int,litt> mElementDead;
    bool isSatisfied();
    void merge(clause* cl);
};

#endif // CLAUSE_H
