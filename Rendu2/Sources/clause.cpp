#include "../Header/clause.h"
void clause::set_clause(litt* litt_entry){
    this->nbrLitt=1;
    this->ElementAlive=(litt**)malloc(sizeof(litt*));
    this->ElementAlive[0]=litt_entry;
    this->ElementDead=(litt**)malloc(sizeof(litt*));
    this->ElementDead[0]=nullptr;

	this->mElementAlive[0]=litt_entry;
	this->mElementDead[0]=nullptr;
    this->id=0;
}


void clause::merge(clause* cl2){
    //TODO : listes
    for (auto& s:cl2->mElementAlive)
        this->mElementAlive[this->mElementAlive.size()]=s.second;
    for (auto& s:cl2->mElementDead)
        this->mElementDead[this->mElementDead.size()]=s.second;
    for (auto& s:v_var)
        if (s!=nullptr){
            for (auto& s2:s->clauseInto)
                if (s2 == cl2)
                    s2 = this;
       }
    delete cl2;
}

int clause::nbLittAlive(){
	int result=0;
	for (auto& li:this->mElementAlive)
		if (li.second != nullptr)
			result++;
	return result;
}

litt* clause::getUniqueLittAlive(){
    //renvoie un pointeur vers l'unique littéral de la clause s'il existe
    //et est unique. Sinon, renvoie nullptr.
	int result=0;
	litt* unique_litt;
	for (auto& li:this->mElementAlive)
		if (li.second != nullptr){
			result++;
			unique_litt=li.second;
		}
	if (result == 1)
		return unique_litt;
	else
		return nullptr;
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

void clause::free_clause(){
    for (auto& s:this->mElementAlive)
        if (s.second != nullptr){
             delete s.second;
        }
    for (auto& s:this->mElementDead)
        if (s.second != nullptr){
             delete s.second;
        }
}

bool clause::isSatisfied(){
    for (auto& s:this->mElementDead)
        if (s.second != nullptr){
            if (s.second->neg){
                //si nié, le litt est vraie si la variable est à 0
                if (s.second->variable->value==0){
                    return true;
                }
            }else{
                //si non nié, le litt est vrai si une variable est à 1
                if (s.second->variable->value==1){
                    return true;
                }
            }
        }
    return false;
}
