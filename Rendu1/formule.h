#include <map>
#include "clause.h"
#ifndef FORMULE_H
#define FORMULE_H
using namespace std;


class formule{
public:
    map<unsigned int,clause> mClauseSatisfied;
    map<unsigned int,clause> mClauseUnsatisfied;
    void merge(formule* formule2);
    void setFromOneVar(int id);
    //merge this and formule 2 and place it into this
};

#endif // FORMULE_H
