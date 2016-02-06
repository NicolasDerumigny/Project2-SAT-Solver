#include "formule.h"

void merge(formule formule2){
    for (auto& s:formule2.mElement_alive)
        this->mElement_alive[this->mElement_alive.size()]=s.second;
    for (auto& s:formule2.mElement_dead)
        this->mElement_dead[this->mElement_dead.size()]=s.second;
}
