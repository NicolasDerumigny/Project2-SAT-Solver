#include <vector>
#include <utility>

void assignUniqueLitt(){
	litt* li;
	for (auto& variable:v_var)
		if (variable != nullptr && variable->value == -1) //on vérifie que la variable est définie, et qu'elle est non assignée
			for (auto& cl:variable->clauseInto){
				li = cl->getUniqueLittAlive(); //Amélioration : vérifier si la clause est de la forme {x1 or x1 or ... or x1}, la même variable avec la même polarité... Ou au pire on peut faire du prétraitement de la clause...
				if (li != nullptr && li->variable == variable){
					if (li->neg == false)
						li->variable->assignValue(1,false);
					else
						li->variable->assignValue(0,false);
				}
			}
}

void assignUniquePolarity(){
	vector<pair<int,int> > variables (v_var.size(), std::make_pair(0,0)); //vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)
	for (auto& cl:instance->mClauseUnsatisfied)
		if (cl != nullptr)
			for (auto& li:cl->mElementAlive)
				if (li != nullptr) {
					if (li->neg == true)
						variables[li->variable->id].first++; //on met à jour le vector variables
					if (li->neg == false)
						variables[li->variable->id].second++;
				}
	int id=0;
	for (auto& variable:variables){ //pour chaque variable non assignée, on vérifie qu'elle n'apparait pas à la fois comme variable niée et non niée
		if (v_var[id] != nullptr && variables[id].first == 0 && variables[id].second != 0)
			v_var[id]->assignValue(1,false);
		if (v_var[id] != nullptr && variables[id].first != 0 && variables[id].second == 0)
			v_var[id]->assignValue(0,false);
	}
}
