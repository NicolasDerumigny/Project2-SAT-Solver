#include "formule.h"
void formule::set_formule_var(var* var, bool neg){
    litt* new_litt = new litt;
    new_litt->set_litt(var, neg);
    clause* new_clause = new clause;
    new_clause->set_clause(new_litt);
    var->clauseInto.push_back(new_clause);
    this->f_ClauseUnsatisfied = new_clause;
    this->l_ClauseUnsatisfied = new_clause;
    this->f_ClauseSatisfied = nullptr;
    this->l_ClauseSatisfied = nullptr;
}


void formule::set_formule(int varid, bool neg){
    var* new_var=nullptr;
    //Afficher une erreur dans le cas ou la variable n'y est pas ET CONTINUER QUAND MEME
    if (varid>=int(v_var.size())){
        int oldsize=int(v_var.size());
        if (!isTseitin){
            cerr<<"Warning : variable number "<<varid<<" not declared in header line (max "<< oldsize - 1;
            cerr<<"), continuing anyway"<<endl;
        }
        for (int i=0; i<varid-oldsize+1;i++)
            v_var.push_back(nullptr);
    }
    if (v_var[varid]==nullptr){
        new_var= new var;
        new_var->set_var(varid);
        v_var[varid]=new_var;
    }else{
        new_var=v_var[varid];
    }
    set_formule_var(new_var, neg);
}

void formule::set_formule_tseitin(bool neg){
    var* new_var = new var;
    new_var->set_var(v_var_tseitin.size());
    v_var_tseitin.push_back(new_var);
    set_formule_var(new_var, neg);
}

void formule::merge(formule* formule2){
    if (formule2->l_ClauseSatisfied != nullptr){//formule2 n'est pas vide
        if (this->l_ClauseSatisfied == nullptr){//formule est vide
            this->f_ClauseSatisfied = formule2->f_ClauseSatisfied;
            this->l_ClauseSatisfied = formule2->l_ClauseSatisfied;
        } else {//formule n'est pas vide
            this->l_ClauseSatisfied->next_clause = formule2->f_ClauseSatisfied;
            this->l_ClauseSatisfied = formule2->l_ClauseSatisfied;
        }
    }
    if (formule2->l_ClauseUnsatisfied != nullptr){
        if (this->l_ClauseUnsatisfied == nullptr){
            this->f_ClauseUnsatisfied = formule2->f_ClauseUnsatisfied;
            this->l_ClauseUnsatisfied = formule2->l_ClauseUnsatisfied;
        } else {
            this->l_ClauseUnsatisfied->next_clause = formule2->f_ClauseUnsatisfied;
            this->l_ClauseUnsatisfied = formule2->l_ClauseUnsatisfied;
        }
    }
//    for (auto& s:formule2->mClauseSatisfied){
//        this->mClauseSatisfied[this->mClauseSatisfied.size()]=s.second;
//        if (s.second != nullptr)
//            this->mClauseSatisfied[this->mClauseSatisfied.size()-1]->id=this->mClauseSatisfied.size()-1;
//    }
//    for (auto& s:formule2->mClauseUnsatisfied){
//        this->mClauseUnsatisfied[this->mClauseUnsatisfied.size()]=s.second;
//        if (s.second != nullptr)
//            this->mClauseUnsatisfied[this->mClauseUnsatisfied.size()-1]->id=this->mClauseUnsatisfied.size()-1;
//    }
    delete formule2;
}

void formule::print(){
    cout<<"Satisfied :"<<endl;
	for (clause* cl = this->f_ClauseSatisfied;cl != nullptr;cl=cl->next_clause){
		cl->print();
	}
//    for (auto& s:this->mClauseSatisfied)
//        if (s.second != nullptr)
//			s.second->print();
    cout<<"Unsatisfied :"<<endl;
	for (clause* cl = this->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause){
		cl->print();
	}
//    for (auto& s:this->mClauseUnsatisfied)
//        if (s.second != nullptr)
//			s.second->print();
    cout<<endl<<endl;

}

void formule::free_formule(){
	clause* cl = this->f_ClauseSatisfied;
	clause* old_cl = nullptr;
	while (cl != nullptr){
		old_cl = cl;
		cl = cl->next_clause;
		old_cl->free_clause();
		delete old_cl;
	}
	cl = this->f_ClauseUnsatisfied;
	old_cl = nullptr;
	while (cl != nullptr){
		old_cl = cl;
		cl = cl->next_clause;
		old_cl->free_clause();
		delete old_cl;
	}
//    for (auto& s:this->mClauseSatisfied)
//        if (s.second != nullptr){
//             s.second->free_clause();
//             delete s.second;
//        }
//    for (auto& s:this->mClauseUnsatisfied)
//        if (s.second != nullptr){
//             s.second->free_clause();
//             delete s.second;
//        }
}

/* ----------- formula preprocessing --------------- */
void formule::preprocessing() {
	//la détection des clauses unitaires se fait via la function assignUniqueLitt() de deduce.cpp
	//élimination des doublons (vivants) et des clauses tautologiques (non satisfaites)
	vector<pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
	//vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)
	bool isTauto;
	bool li_need_back;
	litt* li_prev;
	clause* cl_prev = nullptr;
	bool cl_need_back = false;
	for (clause* cl=this->f_ClauseUnsatisfied;cl != nullptr || cl_need_back;cl=cl->next_clause) {
        if (cl_need_back){
			cl=cl_prev;
			cl_prev=nullptr;
			cl_need_back = false;
		}
		li_need_back = false;
		li_prev = nullptr;
		for (litt* li = cl->f_ElementAlive;li != nullptr || li_need_back;li = li->next_litt) {
			if (li_need_back){
				li=li_prev;
				li_prev=nullptr;
				li_need_back = false;
			}
			if (li->neg){
				if (variables[li->variable->id].first > 0){//si on a un doublon dans la clause, on l'élimine
					removeLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li,li_prev);
					delete li;
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementAlive != nullptr){
						li = cl->f_ElementAlive;
						li_need_back = true;
					} else//there is nothing left
						break;
				} else { 
					variables[li->variable->id].first++;
				}
			} else {
				if (variables[li->variable->id].second > 0){//si on a un doublon dans la clause, on l'élimine
					removeLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li,li_prev);
					delete li;
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementAlive != nullptr){
						li = cl->f_ElementAlive;
						li_need_back = true;
					} else//there is nothing left
						break;
				} else { 
					variables[li->variable->id].second++;
				}
			}
			li_prev = li;
		}
		isTauto = false;
		for (auto& v:variables){
			if (v.first != 0 and v.second != 0){//la clause est tautologique
				isTauto = true;
			}
			v.first = 0;
			v.second = 0;
		}
		if (isTauto) {
			removeClause(&this->f_ClauseUnsatisfied,&this->l_ClauseUnsatisfied,cl,cl_prev);
			for (litt* li = cl->f_ElementAlive;li != nullptr;li = li->next_litt){//supprimer la clause cl des clauseInto
				li->variable->clauseInto.erase(std::remove(li->variable->clauseInto.begin(), li->variable->clauseInto.end(), cl), li->variable->clauseInto.end());
			}
			cl->free_clause();
			if (cl_prev != nullptr)
				cl = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
			else if (this->f_ClauseUnsatisfied != nullptr){
				cl = this->f_ClauseUnsatisfied;
				cl_need_back = true;
			} else//there is nothing left
				break;
		}
		cl_prev = cl;
	}
	//supprimer les doublons de clauseInto
	for (auto& v:v_var){
		if (v != nullptr){
			sort(v->clauseInto.begin(), v->clauseInto.end());
			v->clauseInto.erase(std::unique(v->clauseInto.begin(), v->clauseInto.end()), v->clauseInto.end());
		}
	}
}
