#include "var.h"

void var::print(){
    //affiche juste l'affectation, pas d'espace ni de endl
    switch (this->value){
        case 1:
            cout<<this->id;
            break;

        case 0:
            cout<<"-"<<this->id;
            break;

       case -1:
            cout<<"±"<<this->id;
            break;
   }
}


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

void var::assignValue(int value,bool bet) {
	this->value=value;
//On met à jour les variables mortes/vivantes, vis-à-vis du passage de variable de vivant à mort.
	this->updateStatus(false);
//On ajoute cette assignation dans la pile d'assignations en précisant qu'il s'agit d'un pari.
	assignation* new_assign;
	new_assign = new assignation;
	new_assign->set_assign(this,bet);
	assignations.push_back(new_assign);
}
