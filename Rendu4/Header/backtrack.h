#ifndef BACKTRACK_H
#define BACKTRACK_H
#include "formule.h"
#include "global_variable_extern.h"
#include "assignation.h"
#include "prooftree.h"
#include <stdlib.h>


clause* getConflict();
bool backtrack();
bool conflictAnal(clause *cl_Conflict);
clause* getUIPClause(clause *cl_Conflict);
bool checkAssign();

#endif // BACKTRACK_H
