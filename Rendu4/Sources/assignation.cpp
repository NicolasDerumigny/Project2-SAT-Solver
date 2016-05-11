#include "assignation.h"


void assignation::set_assign(var* variable_enter, bool bet_enter) {
    bets.push_back(bet_enter);
    this->variable=variable_enter;
    this->bet=bet_enter;
}

void assignation::updateLitt(bool alive){
	bool li_need_back = false;
	litt* li_prev = nullptr;
    for (auto& cl:this->variable->clauseInto){
        if (alive == false) { //si on tue une variable, on recherche les littéraux associés dans les éléments vivants et on les transfères vers les morts.
            li_need_back = false;
			li_prev = nullptr;
            for (litt* li = cl->f_ElementAlive;li != nullptr or li_need_back;li=li->next_litt){
                //si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li_need_back){
					li=li_prev;
					li_prev=nullptr;
					li_need_back = false;
				}
                if (li->variable == this->variable) {
                    removeLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li,li_prev);
                    appendLitt(&cl->f_ElementDead,&cl->l_ElementDead,li);
                    if (needNewWatched(cl,li))
                        assignNewWatched(cl,li);
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementAlive != nullptr){//on est au début
						li = cl->f_ElementAlive;
						li_need_back = true;
					} else//there is nothing left
                        break;
                }
                //renvoie false si on n'a pas trouvé d'autre litteral possible
                //cependant, si rien n'a été trouvé, on fera bien le backtrack
				li_prev = li;
			}
//			for (auto& li:cl->mElementAlive)//si un littéral (donc la variable) est déjà mort on ne fait rien.
//                if (li.second != nullptr && li.second->variable == this->variable) {
//                    cl->mElementDead[li.first] = li.second;
//                    li.second = nullptr;
//				}
        } else { //et réciproquement...
            li_need_back = false;
			li_prev = nullptr;
			for (litt* li = cl->f_ElementDead;li != nullptr || li_need_back;li=li->next_litt) {//si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li_need_back){
					li=li_prev;
					li_prev=nullptr;
					li_need_back = false;
				}
                if (li->variable == this->variable) {
                    removeLitt(&cl->f_ElementDead,&cl->l_ElementDead,li,li_prev);
                    appendLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li);
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementDead != nullptr) {
						li = cl->f_ElementDead;
						li_need_back = true;
					} else//there is nothing left
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
}

void assignation::updateClause(bool alive){
	bool cl_need_back = false;
	clause* cl_prev = nullptr;
    for (auto& cl:this->variable->clauseInto)
        if (alive == false) {
            //si on assigne (on tue) une variable, on recherche les clauses associés qui sont encore non satisfaites, et on les met à jour
			cl_need_back = false;
			cl_prev = nullptr;
			for (clause* cl2 = instance->f_ClauseUnsatisfied;cl2 != nullptr || cl_need_back;cl2=cl2->next_clause){//On parcours les clauses non satisfaites à la recherche de cl
				if (cl_need_back){
					cl2=cl_prev;
					cl_prev=nullptr;
					cl_need_back = false;
				}
				if (cl2 == cl)
                    if (cl2->isSatisfied()){
                        //on enlève cl2 de la liste simplement chainée des clauses non satisfaites, puis on l'ajoute aux clauses satisfaites
                        removeClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,cl2,cl_prev);
                        appendClause(&instance->f_ClauseSatisfied,&instance->l_ClauseSatisfied,cl2);
						if (cl_prev != nullptr)
                            cl2 = cl_prev;
                        //On évite de casser la chaîne de parcours de la boucle for...
						else if (instance->f_ClauseUnsatisfied != nullptr){
							cl2 = instance->f_ClauseUnsatisfied;
							cl_need_back = true;
						} else//there is nothing left
							break;
                    }
				cl_prev = cl2;
            }
        } else {
            //et réciproquement...
			cl_need_back = false;
			cl_prev = nullptr;
			for (clause* cl2 = instance->f_ClauseSatisfied;cl2 != nullptr || cl_need_back;cl2=cl2->next_clause){//On parcours les clauses non satisfaites à la recherche de cl
				if (cl_need_back){
					cl2=cl_prev;
					cl_prev=nullptr;
					cl_need_back = false;
				}
				if (cl2 == cl)
					if (!cl2->isSatisfied()){//on enlève cl2 de la liste simplement chainée des clauses satisfaites, puis on l'ajoute aux clauses non satisfaites
                        removeClause(&instance->f_ClauseSatisfied,&instance->l_ClauseSatisfied,cl2,cl_prev);
                        appendClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,cl2);
						if (cl_prev != nullptr)
							cl2 = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
						else if (instance->f_ClauseSatisfied != nullptr){
							cl2 = instance->f_ClauseSatisfied;
							cl_need_back = true;
						} else//there is nothing left
							break;
                    }
                if (wl && cl2->existsWatchedNonAlive() && cl2->isSatisfied())
                    std::cerr<<"Error : There exist false watched litterals in an alive clause"<<std::endl;
                cl_prev = cl2;
            }
        }
}

void assignation::updateStatus(bool alive){
    this->updateLitt(alive);
    this->updateClause(alive);
}


void assignation::print(){
    this->variable->print();
}

bool needNewWatched(clause * cl, litt* li){
    if (cl->w_litt_1==li or cl->w_litt_2==li)
        return true;
    else
        return false;
}

bool assignNewWatched(clause * cl, litt* li){
    bool changed=false;
    if (cl->w_litt_1==li or cl->w_litt_2==li){
        cl->w_litt_1=(cl->w_litt_1==li)?cl->w_litt_2:cl->w_litt_1;
        //li est maintenant le litteral watched numero 2
        for(litt* li2=cl->f_ElementAlive;li2!=nullptr; li2=li2->next_litt){
            if (li2!=cl->w_litt_1 and li2!=li){
                cl->w_litt_2=li2;
                changed=true;
                break;
            }
        }
        if (!changed){
            //si jamais on n'a pas réussi a changer, on va bouger sur un litteral mort
            //(au cas ou on supprime le litteral en question
            for(litt* li2=cl->f_ElementAlive;li2!=nullptr; li2=li2->next_litt){
                if (li2!=cl->w_litt_1 and li2!=li){
                    cl->w_litt_2=li2;
                    changed=true;
                    break;
                }
            }

        }

    }else
        return true;
    return changed;
}
