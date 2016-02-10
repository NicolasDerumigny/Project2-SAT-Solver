#include "clause.h"
void clause::set_clause(litt litt_entry){
    map<unsigned int,litt> mElementAlive_Entry;
    map<unsigned int,litt> arg_null;
    mElementAlive_Entry[0]=litt_entry;
    this->mElementAlive= mElementAlive_Entry;
    this->mElementDead= arg_null;
}


void clause::merge(clause* cl2){
    for (auto& s:cl2->mElementAlive)
        this->mElementAlive[this->mElementAlive.size()]=s.second;
    for (auto& s:cl2->mElementDead)
        this->mElementDead[this->mElementDead.size()]=s.second;
}
