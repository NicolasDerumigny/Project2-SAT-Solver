#include "../Header/clause.h"
void clause::set_clause(litt* litt_entry){
    this->f_ElementAlive=litt_entry;
    this->f_ElementDead=nullptr;
    this->l_ElementAlive=litt_entry;
    this->l_ElementDead=nullptr;
    //this->mElementAlive[0]=litt_entry;
    //this->mElementDead[0]=nullptr;
    this->id=0;
}


void clause::merge(clause* cl2){
    if(this->l_ElementAlive!=nullptr){
        this->l_ElementAlive->next_litt=cl2->f_ElementAlive;
    }else{
        this->f_ElementAlive=cl2->f_ElementAlive;
    }
    this->l_ElementAlive=cl2->l_ElementAlive;

    if(this->l_ElementDead!=nullptr){
        this->l_ElementDead->next_litt=cl2->f_ElementDead;
    }else{
        this->f_ElementDead=cl2->f_ElementDead;
    }
    this->l_ElementDead=cl2->l_ElementDead;
    //fusion des listes simplement chainées

    //Ancien : maps
    /*for (auto& s:cl2->mElementAlive)
        this->mElementAlive[this->mElementAlive.size()]=s.second;
    for (auto& s:cl2->mElementDead)
        this->mElementDead[this->mElementDead.size()]=s.second;*/
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
    for(litt* course=this->f_ElementAlive;course!=nullptr;course=course->next_litt){
        result++;
    }
    /*for (auto& li:this->mElementAlive)
		if (li.second != nullptr)
            result++;*/
	return result;
}

litt* clause::getUniqueLittAlive(){
    //renvoie un pointeur vers l'unique littéral de la clause s'il existe
    //et est unique. Sinon, renvoie nullptr.
    if (this->f_ElementAlive!= nullptr and this->f_ElementAlive==this->l_ElementAlive)
        return this->f_ElementAlive;
    else
        return nullptr;
    /*
    int result=0;
    for (auto& li:this->mElementAlive)
		if (li.second != nullptr){
			result++;
			unique_litt=li.second;
        }
	if (result == 1)
		return unique_litt;
	else
        return nullptr;*/
}

void clause::print(){
    cout<<"\tElements vivants :"<<endl;
    for(litt* course=this->f_ElementAlive;course!=nullptr;course=course->next_litt){
        course->print();
    }
    /*
     for (auto& s:this->mElementAlive)
        if (s.second != nullptr)
			s.second->print();
     */
    cout<<endl;
    cout<<"\tElements morts :"<<endl;
    for(litt* course=this->f_ElementDead;course!=nullptr;course=course->next_litt){
        course->print();
    }
    /*
    for (auto& s:this->mElementDead)
        if (s.second != nullptr)
			s.second->print();
            */
    cout<<"----------------------------";
    cout<<endl;
}

void clause::free_clause(){
    litt* course=this->f_ElementAlive;
    litt* old_litt=nullptr;
    while (course!=nullptr){
        old_litt=course;
        course=course->next_litt;
        delete old_litt;
    }

    course=this->f_ElementDead;
    old_litt=nullptr;
    while (course!=nullptr){
        old_litt=course;
        course=course->next_litt;
        delete old_litt;
    }

    /*for (auto& s:this->mElementAlive)
        if (s.second != nullptr){
             delete s.second;
        }
    for (auto& s:this->mElementDead)
        if (s.second != nullptr){
             delete s.second;
        }*/
}

bool clause::isSatisfied(){
    for(litt* course=this->f_ElementDead;course!=nullptr;course=course->next_litt){
        if (course->neg){
            if (course->variable->value==0){
                return true;
            }
        }else{
            if (course->variable->value==1){
                return true;
            }
        }
    }



    /*for (auto& s:this->mElementDead)
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
        }*/
    return false;
}
