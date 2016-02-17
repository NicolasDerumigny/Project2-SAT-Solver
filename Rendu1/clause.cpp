#include "clause.h"
void clause::set_clause(litt* litt_entry){
	this->mElementAlive[0]=litt_entry;
	this->mElementDead[0]=nullptr;
}


void clause::merge(clause* cl2){
    for (auto& s:cl2->mElementAlive)
        this->mElementAlive[this->mElementAlive.size()]=s.second;
    for (auto& s:cl2->mElementDead)
        this->mElementDead[this->mElementDead.size()]=s.second;
    for (auto& s:v_var)
        for (auto& s2:s->clauseInto)
            if (s2 == cl2)
                s2 = this;
}

void clause::print(){
    cout<<"\tElements vivants :"<<endl;
    for (auto& s:this->mElementAlive)
        if (s.second != nullptr)
			s.second->print();
    cout<<endl;
    cout<<"\tElements morts :"<<endl;
    for (auto& s:this->mElementDead)
        if (s.second != nullptr)
			s.second->print();
    cout<<"----------------------------";
    cout<<endl;
}
