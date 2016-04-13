#ifndef PROOFTREE_H
#define PROOFTREE_H
#include <stdio.h>
#include "global_variable_extern.h"
#include "assignation.h"
#include "formule.h"

void setNewProofTree();
void writingAssign(assignation* ass);
void deleteProofTree();
void writeAxiom();
void writeBinary();
void writeDeduction(assignation* ass);

#endif // PROOFTREE_H
