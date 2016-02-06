#include "clause.h"

void merge(clause* cl2){
    for (auto& s:*cl2.mElementAlive)
        this->mElementAlive[this->mElementAlive.size()]=s.second;
    for (auto& s:*cl2.mElementDead)
        this->mElementDead[this->mElementDead.size()]=s.second;
}
