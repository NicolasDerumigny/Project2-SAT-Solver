#ifndef FORMULE_H
#define FORMULE_H
#include <map>
using namespace std;

class formule{
public:
    void set_formule(int varid, bool neg);
    void print();
    void free_formule();

    map<unsigned int,clause*> mClauseSatisfied;
    map<unsigned int,clause*> mClauseUnsatisfied;
    void merge(formule* formule2);
    //merge this and formule 2 and place it into this
};

#endif // FORMULE_H
