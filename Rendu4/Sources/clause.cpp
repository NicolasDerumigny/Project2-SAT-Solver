#include "clause.h"
#include "litt.h"

void clause::set_clause(litt* litt_entry){
    this->f_ElementAlive=litt_entry;
    this->f_ElementDead=nullptr;
    this->l_ElementAlive=litt_entry;
    this->l_ElementDead=nullptr;
    this->satisfied=false;
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
                cl2->f_ElementAlive->prev_litt=this->l_ElementAlive;
            }else{
                this->f_ElementAlive=cl2->f_ElementAlive;
            }
            this->l_ElementAlive=cl2->l_ElementAlive;
        }

        if (cl2->l_ElementDead!=nullptr){
            if(this->l_ElementDead!=nullptr){
                this->l_ElementDead->next_litt=cl2->f_ElementDead;
                cl2->f_ElementDead->prev_litt=this->l_ElementDead;
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
    return nullptr;
}

void clause::print(){
    std::cout<<"\n------------------------------------------------------\n";
    std::cout<<"\tAlive :  ";
    for(litt* course=this->f_ElementAlive;course!=nullptr;course=course->next_litt){
        course->print();
    }
    /*
     for (auto& s:this->mElementAlive)
        if (s.second != nullptr)
			s.second->print();
     */
    std::cout<<std::endl;
    std::cout<<"\tDead :   ";
    for(litt* course=this->f_ElementDead;course!=nullptr;course=course->next_litt){
        course->print();
    }

    if (this->w_litt_1!=nullptr){
        std::cout<<"\n Watching :"<<std::endl;
        (this->w_litt_1)->print();
        (this->w_litt_2)->print();
        std::cout<<std::endl;
    }


    std::cout<<"\n------------------------------------------------------";
    std::cout<<std::endl;
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
    if(wl){
        if(this->w_litt_1->variable->value==0 and this->w_litt_1->neg==true)
            return true;
        if(this->w_litt_1->variable->value==1 and this->w_litt_1->neg==false)
            return true;
        if(this->w_litt_2->variable->value==0 and this->w_litt_2->neg==true)
            return true;
        if(this->w_litt_2->variable->value==1 and this->w_litt_2->neg==false)
            return true;
        return false;
    }else{
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
        return false;
    }
}

void removeClause(clause **first_cl,clause **last_cl,clause *cur_cl) {
    if (cur_cl == *first_cl){//On est au début de la liste
        *first_cl = cur_cl->next_clause;
	} else {
        if (cur_cl->prev_clause == nullptr) fprintf(stderr, "Fatal: An unexpected error occured in removeClause (prev_cl==nullptr but cur_cl!=first_cl)");
        cur_cl->prev_clause->next_clause = cur_cl->next_clause;
	}
    if (cur_cl == *last_cl){//On est à la fin de la liste
        *last_cl = cur_cl->prev_clause;
    } else {
        if (cur_cl->next_clause == nullptr) fprintf(stderr, "Fatal: An unexpected error occured in removeClause (next_cl==nullptr but cur_cl!=last_cl)");
        cur_cl->next_clause->prev_clause = cur_cl->prev_clause;
	}
}

void appendClause(clause **first_cl, clause **last_cl,clause *cur_cl) {
    if (*last_cl == nullptr){//Il n'y a pas encore de clauses
        *first_cl = cur_cl;
        *last_cl = cur_cl;
        (*last_cl)->next_clause = nullptr;
        (*first_cl)->prev_clause = nullptr;
	} else {
        (*last_cl)->next_clause = cur_cl;
        cur_cl->prev_clause = *last_cl;
        *last_cl = cur_cl;
        (*last_cl)->next_clause = nullptr;
	}
}

bool clause::existsWatchedNonAlive(){
    //renvoie false si un des watched litterals est a vrai ou si les deux sont non assignés
    if(this->w_litt_1->variable->value==-1 and this->w_litt_1->variable->value==-1)
        return false;
    if(this->w_litt_1->variable->value==0 and this->w_litt_1->neg==true)
        return false;
    if(this->w_litt_1->variable->value==1 and this->w_litt_1->neg==false)
        return false;
    if(this->w_litt_2->variable->value==0 and this->w_litt_2->neg==true)
        return false;
    if(this->w_litt_2->variable->value==1 and this->w_litt_2->neg==false)
        return false;
    return true;
}

clause* clause::copy(){
    clause* new_clause=new clause;
    litt* new_litt;
    bool first=true;
    for(litt* li=this->f_ElementAlive;li!=nullptr;li=li->next_litt){
        new_litt=li->copy();
        li->variable->clauseInto.push_back(new_clause);

        if (this->w_litt_1==li)
            new_clause->w_litt_1=new_litt;
        if (this->w_litt_2==li)
            new_clause->w_litt_2=new_litt;


        if (first){
            new_clause->f_ElementAlive=new_litt;//faire une copie d'un littéral c'est bien, l'utiliser c'est mieux...
            new_clause->l_ElementAlive=new_litt;
            first=false;//mettre un compteur booléen c'est bien, l'incrémenter c'est encore mieux...
        }else{
            new_clause->l_ElementAlive->next_litt=new_litt;
            new_litt->prev_litt=new_clause->l_ElementAlive;
            new_clause->l_ElementAlive=new_litt;
        }
    }


    for(litt* li=this->f_ElementDead;li!=nullptr;li=li->next_litt){
        new_litt=li->copy();
        li->variable->clauseInto.push_back(new_clause);

        if (this->w_litt_1==li)
            new_clause->w_litt_1=new_litt;
        if (this->w_litt_2==li)
            new_clause->w_litt_2=new_litt;

        if (first){
            new_clause->f_ElementDead=new_litt;
            new_clause->l_ElementDead=new_litt;
            first=false;
        }else{
            new_clause->l_ElementDead->next_litt=new_litt;
            new_litt->prev_litt=new_clause->l_ElementDead;
            new_clause->l_ElementDead=new_litt;
        }
    }

    return new_clause;
}

std::string clause::proof_str(bool complete){
    std::string answer="";
    bool first=true;
    for(litt* li=this->f_ElementAlive; li!=nullptr;li=li->next_litt){
        if (!first)
            answer = answer + " \\lor " + li->proof_str();
        else
            answer = li->proof_str();
        first=false;
    }
     if (complete){
         for(litt* li=this->f_ElementDead; li!=nullptr;li=li->next_litt){
             if (!first)
                 answer = answer + " \\lor " + li->proof_str();
             else
                 answer = li->proof_str();
             first=false;
         }
     }

    return answer;
}
