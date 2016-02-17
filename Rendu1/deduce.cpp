#include <vector>

void assignUniqueLitt() {
	litt* li;
	for (auto& variable:v_var)
		for (auto& cl:variable->clauseInto){
			li = cl->getUniqueLittAlive();//Amélioration : vérifier si la clause est de la forme {x1 or x1 or ... or x1}, la même variable avec la même polarité... Ou au pire on peut faire du prétraitement de la clause...
			if (li != nullptr){
				if (li->neg == false)
					li->variable->assignValue(1,false);
				else
					li->variable->assignValue(0,false);
			}
		}
}
