#ifndef VAR_H
#define VAR_H
#include <map>
#include "clause.h"
#include "assignation.h"
#include "global_variable_extern.h"

class clause;

class var{
public:

    unsigned long id;
    int value;
    //-1 : unassigned, 0=false, 1 = true
    int level_ass = 0;

    float score = 0;


    bool bet = false;
    bool UIP = false;

    clause* clConflict = nullptr;

    std::vector<clause*> clauseInto;
    //amélioration possible : metttre deux map des clauses la contenant et ne la contenant pas
    std::vector<var*> varConflict;


    void set_var(unsigned long id_enter);
    void print();
    void assignValue(int value_enter, bool bet_enter);
};

void set_var_tseitin(var **new_var);

#endif // VAR_H
