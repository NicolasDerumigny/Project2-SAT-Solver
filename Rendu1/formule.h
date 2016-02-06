#include <map>
#include "clause.h"
#ifndef FORMULE_H
#define FORMULE_H

class formule{
public:
    map<unsigned int,clause> m_clause_satisfied;
    map<unsigned int,clause> m_clause_unsatisfied;
    void merge(formule formule2);
    //merge this and formule 2 and place it into this
};

#endif // FORMULE_H
