#include "var.h"

void var::set_var(int id){
    this->id=id;
    this->value=-1;
}

void var::updateStatus(bool alive){
	for (auto& cl:this->clauseInto)
		if (alive == false) { //si on tue la variable, on la cherche dans les éléments vivants et on la transfère vers les morts.
			for (auto& li:cl->mElementAlive)
				if (li.second != nullptr && li.second->variable == this) {//si la variable est déjà morte on ne fait rien.
					cl->mElementDead[li.first] = li.second;
					li.second = nullptr;
				}
		} else { //et réciproquement...
			for (auto& li:cl->mElementDead)
				if (li.second != nullptr && li.second->variable == this) {
					cl->mElementAlive[li.first] = li.second;
					li.second = nullptr;
				}
		}
}
