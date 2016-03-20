#include <vector>
#include <utility>

bool assignUniqueLitt(){
	bool haveChanged = false;
	litt* li;
	for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause){
        if (cl != nullptr){
            li = cl->getUniqueLittAlive(); //Amélioration : vérifier si la clause est de la forme {x1 or x1 or ... or x1}, la même variable avec la même polarité... Ou au pire on peut faire du prétraitement de la clause...
            if (li != nullptr){
                if (li->variable->value != -1){
                    cerr << "Warning: clause contains an assigned litteral that is still alive\n";
                    cl->print();
				}
                if (li->neg == false)
                    li->variable->assignValue(1,false);
                else
                    li->variable->assignValue(0,false);
                haveChanged = true;
            }
        }
	}
	return haveChanged;
}

bool assignUniquePolarity(){
	bool haveChanged = false;
    vector<pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
    //vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)
    for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause)
// 	for (auto& cl:instance->mClauseUnsatisfied)
		if (cl != nullptr)
            for(litt* course=cl->f_ElementAlive;course!=nullptr;course=course->next_litt){
                if (course->neg == true)
                    variables[course->variable->id].first++; //on met à jour le vector variables
                if (course->neg == false)
                    variables[course->variable->id].second++;
            }

            /*for (auto& li:cl.second->mElementAlive)
				if (li.second != nullptr) {
					if (li.second->neg == true)
						variables[li.second->variable->id].first++; //on met à jour le vector variables
					if (li.second->neg == false)
						variables[li.second->variable->id].second++;
                }*/
	int id=0;
	for (auto& variable:variables){ //pour chaque variable non assignée, on vérifie qu'elle n'apparait pas à la fois comme variable niée et non niée
		if (v_var[id] != nullptr && variable.first == 0 && variable.second != 0){
			v_var[id]->assignValue(1,false);
			haveChanged=true;
		}
		if (v_var[id] != nullptr && variable.first != 0 && variable.second == 0){
			v_var[id]->assignValue(0,false);
			haveChanged=true;
		}
	}
	return haveChanged;
}
