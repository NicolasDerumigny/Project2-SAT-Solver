#include "formule.h"

void merge(formule* formule2){
    for (auto& s:*formule2.mClauseSatisfied)
        this->mClauseSatisfied[this->mClauseSatisfied.size()]=s.second;
    for (auto& s:*formule2.mClauseUnsatisfied)
        this->mClauseUnsatisfied[this->mClauseUnsatisfied.size()]=s.second;
}

void setFromOneVar(int value){
    var * contante;
    constante = new var;
    constante.
}
