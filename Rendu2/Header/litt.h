#ifndef LITT_H
#define LITT_H
#include <map>
using namespace std;
class var;

class litt{

public:
    void set_litt(var* var_entry, bool neg);
    void print();
    void free_litt();
    void appendLitt(litt **first_li,litt **last_li,litt *cur_li);
    void removeLitt(litt **first_li,litt **last_li,litt *cur_li,litt *prev_li);

    var* variable;
    bool neg;
    litt* next_litt=nullptr;
    //struct of chained list
};

#endif // LITT_H
