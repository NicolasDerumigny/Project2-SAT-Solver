#include "assignation.h"

void assignation::set_assign(var* variable,bool bet) {
	this->variable=variable;
	this->bet=bet;
}

void assignation::updateLitt(bool alive){
	for (auto& cl:this->variable->clauseInto)
		if (alive == false) { //si on tue une variable, on recherche les littéraux associés dans les éléments vivants et on les transfères vers les morts.
			for (auto& li:cl->mElementAlive)//si un littéral (donc la variable) est déjà mort on ne fait rien.
				if (li.second != nullptr && li.second->variable == this->variable) {
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

void assignation::updateClause(bool alive){
	for (auto& cl:this->variable->clauseInto)
		if (alive == false) { //si on assigne (on tue) une variable, on recherche les clauses associés qui sont encore non satisfaites
			
		} else {
			
		}
}
