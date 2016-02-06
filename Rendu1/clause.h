#include <map>
#include "litt.h"
#ifndef CLAUSE_H
#define CLAUSE_H

class clause{
    map<litt> mElement_alive;
    map<litt> mElement_dead;
    bool isSatisfied();
};

#endif // CLAUSE_H
