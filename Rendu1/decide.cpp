#include <vector>
#include "assignation.h"
//#include <stdlib.h>
//#include <time.h>

var* getFreeVar() {
	for (auto& s:v_var) {
        if (s != nullptr && s->value == -1) {
			return s;
		}
	}
	return nullptr;
}

void assignValue(var* variable) {
	variable->value=1;//Par défaut on assigne à la variable la valeur 1.
//On met à jour les variables mortes/vivantes, vis-à-vis du passage de variable de vivant à mort.
	variable->updateStatus(false);
//On ajoute cette assignation dans la pile d'assignations en précisant qu'il s'agit d'un pari.
	assignation* new_assign;
	new_assign = new assignation;
	new_assign->set_assign(variable,true);
	assignations.push_back(new_assign);
}
