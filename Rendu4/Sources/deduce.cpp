#include "deduce.h"


bool assignUnique(){
    if (!interactive)
        return assignUniqueLitt() or assignUniquePolarity();
    else
        return assignUniqueLitt();
}

bool assignUniqueLitt(){
	bool haveChanged = true;
	litt* li;
	while (haveChanged){
		haveChanged = false;
        for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause){
            li = cl->getUniqueLittAlive();
            if (li != nullptr){
                if (li->variable->value != -1){
                    std::cerr << "Warning: clause contains an assigned litteral that is still alive\n\n";
                    std::cerr << "Litteral is :";
                    li->print();
                    cl->print();
                    std::cerr << "\n\n\n";
                    exit(-1);
                }
                if (clLearning){
                    li->variable->clConflict = cl;
                }
                if (interactive){
                    for (litt* li2=cl->f_ElementDead;li2 != nullptr;li2=li2->next_litt)
                        li->variable->varConflict.push_back(li2->variable);
                    sort(li->variable->varConflict.begin(), li->variable->varConflict.end());
                    li->variable->varConflict.erase(std::unique(li->variable->varConflict.begin(), li->variable->varConflict.end()), li->variable->varConflict.end());
                }
                if (li->neg == false)
                    li->variable->assignValue(1,false);
                else{
                    li->variable->assignValue(0,false);
                }
                haveChanged = true;
                break;
		    }
		}
	}
	return haveChanged;
}
bool assignUniquePolarity(){
    if (clLearning)
        fprintf(stderr,"Warning: Unique polarity deduction while learning from clauses");
    bool haveChanged = false;
    std::vector<std::pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
    //vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)
    for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause)
		if (cl != nullptr)
            for(litt* course=cl->f_ElementAlive;course!=nullptr;course=course->next_litt){
                if (course->neg == true)
                    variables[course->variable->id].first++; //on met à jour le vector variables
                if (course->neg == false)
                    variables[course->variable->id].second++;
            }
    unsigned long id=0;
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
