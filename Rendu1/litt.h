#include <map>
#include "var.h"
#ifndef LITT_H
#define LITT_H
using namespace std;

class litt{
public:
    var* variable;
    bool neg;
    int eval();
};

#endif // LITT_H
