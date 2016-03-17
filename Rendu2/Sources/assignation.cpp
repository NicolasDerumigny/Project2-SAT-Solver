#include "../Header/assignation.h"

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
    // Amélioration : au lieu de revérifier s'il existe un littéral qui satifait la clause (méthode isSatisfied), if faudrait uniquement vérifier les littéraux associées à la variable
clause* cl_prev = nullptr;
    for (auto& cl:this->variable->clauseInto)
        if (alive == false) {
            //si on assigne (on tue) une variable, on recherche les clauses associés qui sont encore non satisfaites, et on les met à jour
			cl_prev = nullptr;
			for (clause* cl2 = instance->f_ClauseUnsatisfied;cl2 != nullptr;cl2=cl2->next_clause){//On parcours les clauses non satisfaites à la recherche de cl
				if (cl2 == cl)
					if (cl2->isSatisfied()){//on enlève cl2 de la liste simplement chainée des clauses non satisfaites, puis on l'ajoute aux clauses satisfaites
						if (cl2 == instance->f_ClauseUnsatisfied){//On est au début de la liste
							instance->f_ClauseUnsatisfied = cl2->next_clause;
						} else {
							cl_prev->next_clause = cl2->next_clause;
						}
						if (cl2 == instance->l_ClauseUnsatisfied){//On est à la fin de la liste
							instance->l_ClauseUnsatisfied = cl_prev;
						}
						if (instance->l_ClauseSatisfied == nullptr){//Il n'y a pas encore de clauses satisfaites
							instance->f_ClauseSatisfied = cl2;
							instance->l_ClauseSatisfied = cl2;
						} else {
							instance->l_ClauseSatisfied->next_clause = cl2;
							instance->l_ClauseSatisfied = cl2;
						}
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
						if (cl2 == instance->f_ClauseSatisfied){//On est au début de la liste
							instance->f_ClauseSatisfied = cl2->next_clause;
						} else {
							cl_prev->next_clause = cl2->next_clause;
						}
						if (cl2 == instance->l_ClauseSatisfied){//On est à la fin de la liste
							instance->l_ClauseSatisfied = cl_prev;
						}
						if (instance->l_ClauseUnsatisfied == nullptr){//Il n'y a pas encore de clauses non satisfaites
							instance->f_ClauseUnsatisfied = cl2;
							instance->l_ClauseUnsatisfied = cl2;
						} else {
							instance->l_ClauseUnsatisfied->next_clause = cl2;
							instance->l_ClauseUnsatisfied = cl2;
						}
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
