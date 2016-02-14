#include "var.h"
#include <map>
#ifndef LITT_H
#define LITT_H
using namespace std;

class litt{

public:
    void set_litt(var* var_entry, bool neg);
    void print();

    var* variable;
    bool neg;
};

#endif // LITT_H
