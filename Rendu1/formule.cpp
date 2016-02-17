#include "formule.h"
void formule::set_formule(int varid, bool neg){
    var* new_var;
    //Afficher une erreur dans le cas ou la variable n'y est pas ET CONTINUER QUAND MEME
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
    this->mClauseUnsatisfied[0]=new_clause;
    this->mClauseSatisfied[0]=nullptr;
}


void formule::merge(formule* formule2){
    for (auto& s:formule2->mClauseSatisfied)
        this->mClauseSatisfied[this->mClauseSatisfied.size()]=s.second;
    for (auto& s:formule2->mClauseUnsatisfied)
        this->mClauseUnsatisfied[this->mClauseUnsatisfied.size()]=s.second;
}

void formule::print(){
    cout<<"Clauses satisfaites :"<<endl;
    for (auto& s:this->mClauseSatisfied)
        if (s.second != nullptr)
			s.second->print();
    cout<<"Clauses non satisfaites :"<<endl;
    for (auto& s:this->mClauseUnsatisfied)
        if (s.second != nullptr)
			s.second->print();

}
