#ifndef VAR_H
#define VAR_H
#include <map>
#include "clause.h"
#include "assignation.h"
#include "global_variable_extern.h"
using namespace std;
class clause;

class var{
public:
    void set_var(int id);
    void print();
	void assignValue(int value,bool bet);

    int id;
    int value;
    //-1 : unassigned, 0=false, 1 = true
    vector<clause*> clauseInto;
    //amélioration possible : metttre deux map des clauses la contenant et ne la contenant pas
    vector<var*> varConflict;
    clause* clConflict = nullptr;
    bool bet = false;
    int level_ass = 0;
    float score = 0;
};

#endif // VAR_H
