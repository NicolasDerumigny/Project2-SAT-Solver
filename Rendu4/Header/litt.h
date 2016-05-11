#ifndef LITT_H
#define LITT_H
#include <map>
#include <stdint.h>
#include "global_variable_extern.h"


class var;

class litt{

public:
    void set_litt(var* var_entry, bool neg_entry);
    void print();
    void free_litt();
    bool isTrue();
    litt* copy();
    std::string proof_str();

    var* variable;
    bool neg;
    litt* next_litt=nullptr;
    litt* prev_litt=nullptr;
    //struct of chained list

};


void appendLitt(litt **first_li,litt **last_li,litt *cur_li);
void removeLitt(litt **first_li,litt **last_li,litt *cur_li);

#endif // LITT_H
