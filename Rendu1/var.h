#include <map>
#ifndef VAR_H
#define VAR_H
using namespace std;

class var{
public:
    int id;
    int value;//-1 : unassigned, 0=false, 1 = true
    int setNewVar(int id);
};

#endif // VAR_H
