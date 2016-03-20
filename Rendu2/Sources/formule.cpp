#include "../Header/formule.h"
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
    litt* new_litt;
    new_litt=new litt;
    new_litt->set_litt(new_var, neg);
    clause* new_clause;
    new_clause= new clause;
    new_clause->set_clause(new_litt);
	new_var->clauseInto.push_back(new_clause);
	this->f_ClauseUnsatisfied = new_clause;
	this->l_ClauseUnsatisfied = new_clause;
	this->f_ClauseSatisfied = nullptr;
	this->l_ClauseSatisfied = nullptr;
//    this->mClauseUnsatisfied[0]=new_clause;
//    this->mClauseSatisfied[0]=nullptr;
}

void formule::set_formule_tseitin(bool neg){
    var* new_var;
    new_var= new var;
    new_var->set_var(v_var_tseitin.size());
    v_var_tseitin.push_back(new_var);
    litt* new_litt;
    new_litt=new litt;
    new_litt->set_litt(new_var, neg);
    clause* new_clause;
    new_clause= new clause;
    new_clause->set_clause(new_litt);
    new_var->clauseInto.push_back(new_clause);
    this->f_ClauseUnsatisfied = new_clause;
    this->l_ClauseUnsatisfied = new_clause;
    this->f_ClauseSatisfied = nullptr;
    this->l_ClauseSatisfied = nullptr;
//    this->mClauseUnsatisfied[0]=new_clause;
//    this->mClauseSatisfied[0]=nullptr;
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
    cout<<"Clauses satisfaites :"<<endl;
	for (clause* cl = this->f_ClauseSatisfied;cl != nullptr;cl=cl->next_clause){
		cl->print();
	}
//    for (auto& s:this->mClauseSatisfied)
//        if (s.second != nullptr)
//			s.second->print();
    cout<<"Clauses non satisfaites :"<<endl;
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
/*void formule::preprocessing() {
	//la détection des clauses unitaires se fait via la function assignUniqueLitt() de deduce.cpp
	//élimination des doublons et des clauses tautologiques
	vector<pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
	//vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)
	for (clause* cl=this->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause) {
        li_prev = nullptr;
		for (litt* li = cl->f_ElementAlive;li != nullptr;li = li->next_litt) {
			if (li->neg){
				if (variables[li->variable->id] != 0){//si on a un doublon dans la clause, on l'élimine
					
				} else { 
					variables[li->variable->id].first++;
				}
			}
		}
	}
}*/

