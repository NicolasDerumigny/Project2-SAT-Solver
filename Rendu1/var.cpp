#include "var.h"

void var::set_var(int id){
    this->id=id;
    this->value=-1;
}

void var::assignValue(int value,bool bet) {
	this->value=value;
//On met à jour les variables mortes/vivantes, vis-à-vis du passage de variable de vivant à mort.
//	this->updateStatus(false);
//On ajoute cette assignation dans la pile d'assignations en précisant qu'il s'agit d'un pari.
	assignation* new_assign;
	new_assign = new assignation;
	new_assign->set_assign(this,bet);
	new_assign->updateStatus()
	assignations.push_back(new_assign);
}
