#include <map>
#include "clause.h"
#ifndef FORMULE_H
#define FORMULE_H

class formule{
public:
    map<clause> m_clause_satisfied;
    map<clause> m_clause_unsatisfied;
};

#endif // FORMULE_H
