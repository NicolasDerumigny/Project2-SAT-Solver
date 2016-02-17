#ifndef CLAUSE_H
#define CLAUSE_H
#include "litt.h"
#include <map>
using namespace std;

class clause{
public:
    void set_clause(litt* litt_entry);
    void print();
	bool isSatisfied();
    void merge(clause* cl);
	void nbLittAlive();
	litt* getUniqueLittAlive();
    void free_clause();


    map<unsigned int,litt*> mElementAlive;
    map<unsigned int,litt*> mElementDead;
};

#endif // CLAUSE_H
