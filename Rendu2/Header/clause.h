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

	litt* next_clause;

    litt* f_ElementAlive;//premier de la liste chainée
    litt* l_ElementAlive;//dernier de la liste chainée
    litt* f_ElementDead;//premier de la liste chainée
    litt* l_ElementDead;//dernier de la liste chainée

    //map<unsigned int,litt*> mElementAlive;
    //map<unsigned int,litt*> mElementDead;
    int id;
};

#endif // CLAUSE_H
