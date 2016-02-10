#include <map>
#ifndef VAR_H
#define VAR_H
using namespace std;

class var{
public:
    int id;
    int value;//-1 : unassigned, 0=false, 1 = true
    void set_var(int id);
    //amélioration possible : metttre deux map des clauses la contenant et ne la contenant pas
};

#endif // VAR_H
