#ifndef VAR_H
#define VAR_H
#include <map>
#include <vector>
#include "clause.h"
#include "assignation.h"
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
};

#endif // VAR_H
