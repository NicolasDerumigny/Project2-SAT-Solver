#include "clause.h"
#include "litt.h"

void clause::set_clause(litt* litt_entry){
    this->f_ElementAlive=litt_entry;
    this->f_ElementDead=nullptr;
    this->l_ElementAlive=litt_entry;
    this->l_ElementDead=nullptr;
    //this->mElementAlive[0]=litt_entry;
    //this->mElementDead[0]=nullptr;
}


void clause::merge(clause* cl2){
    if (cl2!=nullptr){
        for (litt* lit=cl2->f_ElementAlive;lit!=nullptr;lit=lit->next_litt)
            for (auto& s2:lit->variable->clauseInto)
                if (s2 == cl2)
                    s2 = this;

        for (litt* lit=cl2->f_ElementDead;lit!=nullptr;lit=lit->next_litt)
            for (auto& s2:lit->variable->clauseInto)
                if (s2 == cl2)
                    s2 = this;


        if (cl2->l_ElementAlive!=nullptr){
            if(this->l_ElementAlive!=nullptr){
                this->l_ElementAlive->next_litt=cl2->f_ElementAlive;
            }else{
                this->f_ElementAlive=cl2->f_ElementAlive;
            }
            this->l_ElementAlive=cl2->l_ElementAlive;
        }

        if (cl2->l_ElementDead!=nullptr){
            if(this->l_ElementDead!=nullptr){
                this->l_ElementDead->next_litt=cl2->f_ElementDead;
            }else{
                this->f_ElementDead=cl2->f_ElementDead;
            }
            this->l_ElementDead=cl2->l_ElementDead;
        }
        //fusion des listes simplement chainées

        //Ancien : maps
        /*for (auto& s:cl2->mElementAlive)
            this->mElementAlive[this->mElementAlive.size()]=s.second;
        for (auto& s:cl2->mElementDead)
            this->mElementDead[this->mElementDead.size()]=s.second;*/

        delete cl2;
    }
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
    cout<<"\n------------------------------------------------------\n";
    cout<<"\tAlive :  ";
    for(litt* course=this->f_ElementAlive;course!=nullptr;course=course->next_litt){
        course->print();
    }
    /*
     for (auto& s:this->mElementAlive)
        if (s.second != nullptr)
			s.second->print();
     */
    cout<<endl;
    cout<<"\tDead :   ";
    for(litt* course=this->f_ElementDead;course!=nullptr;course=course->next_litt){
        course->print();
    }
    /*
    for (auto& s:this->mElementDead)
        if (s.second != nullptr)
			s.second->print();
            */
    cout<<"\n------------------------------------------------------";
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
//Si la clause est vide on renvoie bien qu'elle est insatisfiable


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

void removeClause(clause **first_cl,clause **last_cl,clause *cur_cl,clause *prev_cl) {
    if (cur_cl == *first_cl){//On est au début de la liste
        *first_cl = cur_cl->next_clause;
	} else {
		if (prev_cl == nullptr) fprintf(stderr, "Fatal: An unexpected error occured in removeClause (prev_cl==nullptr but cur_cl!=first_cl)");
		prev_cl->next_clause = cur_cl->next_clause;
	}
    if (cur_cl == *last_cl){//On est à la fin de la liste
        *last_cl = prev_cl;
        if (*last_cl != nullptr)
            (*last_cl)->next_clause = nullptr;
	}
}

void appendClause(clause **first_cl, clause **last_cl,clause *cur_cl) {
    if (*last_cl == nullptr){//Il n'y a pas encore de clauses
        *first_cl = cur_cl;
        *last_cl = cur_cl;
        (*last_cl)->next_clause = nullptr;
	} else {
        (*last_cl)->next_clause = cur_cl;
        *last_cl = cur_cl;
        (*last_cl)->next_clause = nullptr;
	}
}
