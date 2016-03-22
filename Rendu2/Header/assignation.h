#ifndef ASSIGNATION_H
#define ASSIGNATION_H
#include "global_variable_extern.h"
#include "litt.h"
#include "clause.h"
#include "formule.h"
using namespace std;

class var;
class clause;

class assignation{
public:
    void set_assign(var* variable,bool bet);
	void updateLitt(bool alive);
	void updateClause(bool alive);
	void updateStatus(bool alive);
    void print();

    bool bet;
    //True si l'assignation est dûe à un paris
    var* variable;
};

#endif // ASSIGNATION_H
