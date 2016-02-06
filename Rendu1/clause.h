#include <map>
#include "litt.h"
#ifndef CLAUSE_H
#define CLAUSE_H

class clause{
    map<unsigned int,litt> mElement_alive;
    map<unsigned int,litt> mElement_dead;
    bool isSatisfied();
};

#endif // CLAUSE_H
