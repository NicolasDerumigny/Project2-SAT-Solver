#ifndef CLAUSE_H
#define CLAUSE_H
#include <map>
using namespace std;

class clause{
public:
    void set_clause(litt* litt_entry);
    void print();
    bool isSatisfied();
    void merge(clause* cl);
    int nbLittAlive();
	litt* getUniqueLittAlive();
    void free_clause();

    litt* ElementAlive;//TODO : liste chainée !
    litt* ElementDead;

    map<unsigned int,litt*> mElementAlive;
    map<unsigned int,litt*> mElementDead;
    int id;
};

#endif // CLAUSE_H
