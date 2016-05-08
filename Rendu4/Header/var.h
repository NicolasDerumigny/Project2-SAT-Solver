#ifndef VAR_H
#define VAR_H
#include <map>
#include "clause.h"
#include "assignation.h"
#include "global_variable_extern.h"

class clause;

class var{
public:

    int id;
    int value;
    //-1 : unassigned, 0=false, 1 = true
    int level_ass = 0;

    float score = 0;


    bool bet = false;
    bool UIP = false;

    char padding[6];

    clause* clConflict = nullptr;

    std::vector<clause*> clauseInto;
    //amélioration possible : metttre deux map des clauses la contenant et ne la contenant pas
    std::vector<var*> varConflict;


    void set_var(int id);
    void print();
    void assignValue(int value,bool bet);
};

#endif // VAR_H
