#include "assignation.h"

void assignation::set_assign(var* variable,bool bet) {
	this->variable=variable;
	this->bet=bet;
}

void assignation::updateLitt(bool alive){
	for (auto& cl:this->variable->clauseInto)
		if (alive == false) { //si on tue la variable, on la cherche dans les éléments vivants et on la transfère vers les morts.
			for (auto& li:cl->mElementAlive)
				if (li.second != nullptr && li.second->variable == this->variable) {//si la variable est déjà morte on ne fait rien.
					cl->mElementDead[li.first] = li.second;
					li.second = nullptr;
				}
		} else { //et réciproquement...
			for (auto& li:cl->mElementDead)
				if (li.second != nullptr && li.second->variable == this->variable) {
					cl->mElementAlive[li.first] = li.second;
					li.second = nullptr;
				}
		}
}
