#ifndef BACKTRACK_H
#define BACKTRACK_H
#include "formule.h"
#include "global_variable_extern.h"
#include "assignation.h"


clause* getConflict();
bool backtrack(clause *cl_Conflict);

#endif // BACKTRACK_H
