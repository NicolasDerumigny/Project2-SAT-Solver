#include "litt.h"

void litt::set_litt(var* var_entry, bool neg){
    this->neg=neg;
    this->variable=var_entry;
    this->next_litt=nullptr;
}

void litt::print(){
    if (this->neg){
        std::cout<<" ¬v"<<this->variable->id;
        std::cout<<"("<<this->variable->id<<" = "<<this->variable->value<<") ";
    }
    else{
        std::cout<<" v"<<this->variable->id;
        std::cout<<"("<<this->variable->id<<" = "<<this->variable->value<<") ";
    }
}

litt* litt::copy(){
    litt* new_litt=new litt;
    new_litt->set_litt(this->variable,this->neg);
    return new_litt;
}

void removeLitt(litt **first_li,litt **last_li,litt *cur_li,litt *prev_li) {
    if (cur_li == *first_li){//On est au début de la liste
        *first_li = cur_li->next_litt;
	} else {
		if (prev_li == nullptr) fprintf(stderr, "Fatal: An unexpected error occured in removeLitt (prev_li==nullptr but cur_li!=first_li)");
        prev_li->next_litt = cur_li->next_litt;
	}

    if (cur_li == *last_li){//On est à la fin de la liste
        *last_li = prev_li;
        if (*last_li != nullptr)//il n'etait pas tout seul
            (*last_li)->next_litt = nullptr;
	}
}

void appendLitt(litt **first_li,litt **last_li,litt *cur_li) {
    if (*last_li == nullptr){//Il n'y a pas encore de littéraux
        *first_li = cur_li;
        *last_li = cur_li;
        (*last_li)->next_litt = nullptr;
	} else {
        (*last_li)->next_litt = cur_li;
        *last_li = cur_li;
        (*last_li)->next_litt = nullptr;
	}
}

std::string litt::proof_str(){
    std::string answer="";
    if (this->neg)
        answer="\\neg ";
    answer = answer + "X_{" + std::to_string(this->variable->id) + "}";
    return answer;
}
