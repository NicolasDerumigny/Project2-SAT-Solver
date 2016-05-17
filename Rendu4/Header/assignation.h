#ifndef ASSIGNATION_H
#define ASSIGNATION_H
#include "global_variable_extern.h"
#include "litt.h"
#include "clause.h"
#include "formule.h"
#include <semaphore.h>
#include <pthread.h>


class var;

class assignation{
public:
    void set_assign(var* variable_enter ,bool bet_enter);
	void updateLitt(bool alive);
	void updateClause(bool alive);
	void updateStatus(bool alive);
    void print();

    bool bet;
    //True si l'assignation est dûe à un paris
    var* variable;
};

typedef struct {
    bool alive;
//    sem_t sem_clauseInto;
    var* variable;
} arg_updateLitt;

void* updateLitt_t(void* arg);
bool assignNewWatched(clause * cl, litt* li);
bool needNewWatched(clause * cl, litt* li);
#endif // ASSIGNATION_H
