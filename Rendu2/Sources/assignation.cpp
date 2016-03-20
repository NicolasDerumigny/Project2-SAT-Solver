#include "../Header/assignation.h"

void assignation::set_assign(var* variable,bool bet) {
    this->variable=variable;
    this->bet=bet;
}

void assignation::updateLitt(bool alive){
	litt* li_prev = nullptr;
    for (auto& cl:this->variable->clauseInto)
        if (alive == false) { //si on tue une variable, on recherche les littéraux associés dans les éléments vivants et on les transfères vers les morts.
            li_prev = nullptr;
			for (litt* li = cl->f_ElementAlive;li != nullptr;li=li->next_litt){//si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li != nullptr && li->variable == this->variable) {
                    removeLitt(cl->f_ElementAlive,cl->l_ElementAlive,li,li_prev);
					appendLitt(cl->f_ElementDead,cl->l_ElementDead,li);
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementAlive != nullptr)//on est au début
						li = cl->f_ElementAlive;
					else//there is nothing left
						break;
                }
				li_prev = li;
			}
//			for (auto& li:cl->mElementAlive)//si un littéral (donc la variable) est déjà mort on ne fait rien.
//                if (li.second != nullptr && li.second->variable == this->variable) {
//                    cl->mElementDead[li.first] = li.second;
//                    li.second = nullptr;
//				}
        } else { //et réciproquement...
            li_prev = nullptr;
			for (litt* li = cl->f_ElementDead;li != nullptr;li=li->next_litt) {//si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li != nullptr && li->variable == this->variable) {
                    removeLitt(cl->f_ElementDead,cl->l_ElementDead,li,li_prev);
					appendLitt(cl->f_ElementAlive,cl->l_ElementAlive,li);
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementDead != nullptr)
						li = cl->f_ElementDead;
					else//there is nothing left
						break;
                }
				li_prev = li;
			}
//			for (auto& li:cl->mElementDead)
//                if (li.second != nullptr && li.second->variable == this->variable) {
//                    cl->mElementAlive[li.first] = li.second;
//                    li.second = nullptr;
//                }
        }
}

void assignation::updateClause(bool alive){
    // Amélioration : au lieu de revérifier s'il existe un littéral qui satifait la clause (méthode isSatisfied), if faudrait uniquement vérifier les littéraux associées à la variable
	clause* cl_prev = nullptr;
    for (auto& cl:this->variable->clauseInto)
        if (alive == false) {
            //si on assigne (on tue) une variable, on recherche les clauses associés qui sont encore non satisfaites, et on les met à jour
			cl_prev = nullptr;
			for (clause* cl2 = instance->f_ClauseUnsatisfied;cl2 != nullptr;cl2=cl2->next_clause){//On parcours les clauses non satisfaites à la recherche de cl
				if (cl2 == cl)
					if (cl2->isSatisfied()){//on enlève cl2 de la liste simplement chainée des clauses non satisfaites, puis on l'ajoute aux clauses satisfaites
						removeClause(instance->f_ClauseUnsatisfied,instance->l_ClauseUnsatisfied,cl2,cl_prev);
						appendClause(instance->f_ClauseSatisfied,instance->l_ClauseSatisfied,cl2);
						if (cl_prev != nullptr)
							cl2 = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
						else if (instance->f_ClauseUnsatisfied != nullptr)
							cl2 = instance->f_ClauseUnsatisfied;
						else//there is nothing left
							break;
					}
				cl_prev = cl2;
			}
//            if (instance->mClauseUnsatisfied[cl->id] != nullptr and cl->isSatisfied()) {//si la clause est déjà satisfaite on ne fait rien
//                instance->mClauseSatisfied[cl->id] = instance->mClauseUnsatisfied[cl->id];
//                instance->mClauseUnsatisfied[cl->id] = nullptr;
//            }
        } else {
            //et réciproquement...
			cl_prev = nullptr;
			for (clause* cl2 = instance->f_ClauseSatisfied;cl2 != nullptr;cl2=cl2->next_clause){//On parcours les clauses non satisfaites à la recherche de cl
				if (cl2 == cl)
					if (!cl2->isSatisfied()){//on enlève cl2 de la liste simplement chainée des clauses satisfaites, puis on l'ajoute aux clauses non satisfaites
						removeClause(instance->f_ClauseSatisfied,instance->l_ClauseSatisfied,cl2,cl_prev);
						appendClause(instance->f_ClauseUnsatisfied,instance->l_ClauseUnsatisfied,cl2);
						if (cl_prev != nullptr)
							cl2 = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
						else if (instance->f_ClauseSatisfied != nullptr)
							cl2 = instance->f_ClauseSatisfied;
						else//there is nothing left
							break;
					}
				cl_prev = cl2;
			}
//            if (instance->mClauseSatisfied[cl->id] != nullptr and !cl->isSatisfied()) {
//                instance->mClauseUnsatisfied[cl->id] = instance->mClauseSatisfied[cl->id];
//                instance->mClauseSatisfied[cl->id] = nullptr;
//            }
        }
}

void assignation::updateStatus(bool alive){
    this->updateLitt(alive);
    this->updateClause(alive);
}


void assignation::print(){
    this->variable->print();
}
