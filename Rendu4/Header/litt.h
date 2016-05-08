#ifndef LITT_H
#define LITT_H
#include <map>
#include <stdint.h>
#include "global_variable_extern.h"


class var;

class litt{

public:
    litt* next_litt=nullptr;
    //struct of chained list
    var* variable;
    bool neg;
    char padding[7];

    void set_litt(var* var_entry, bool neg);
    void print();
    void free_litt();
    litt* copy();
    std::string proof_str();
};


void appendLitt(litt **first_li,litt **last_li,litt *cur_li);
void removeLitt(litt **first_li,litt **last_li,litt *cur_li,litt *prev_li);

#endif // LITT_H
