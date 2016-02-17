#ifndef LITT_H
#define LITT_H
#include "var.h"
#include <map>
using namespace std;

class litt{

public:
    void set_litt(var* var_entry, bool neg);
    void print();
    void free_litt();

    var* variable;
    bool neg;
};

#endif // LITT_H
