#ifndef PROOFTREE_H
#define PROOFTREE_H
#include <stdio.h>
#include "global_variable_extern.h"
#include "assignation.h"
#include "formule.h"

void setNewProofTree();
void writeAssign(assignation* ass);
void deleteProofTree();
void writeAxiom();
void writeBinary();
void writeDeduce(assignation* ass);

#endif // PROOFTREE_H
