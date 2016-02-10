#include "formule.h"
void formule::set_formule(int varid, bool neg){
    var* new_var;
    new_var= new var;//en fait non !
    new_var->set_var(varid);
    litt* new_litt;
    new_litt=new litt;
    new_litt->set_litt(new_var, neg);
    clause* new_clause;
    new_clause= new clause;
    new_clause->set_clause(*new_litt);
    this->mClauseUnsatisfied[0]=*new_clause;
    map<unsigned int,clause> atr_null;
    this->mClauseSatisfied=atr_null;
}


void formule::merge(formule* formule2){
    for (auto& s:formule2->mClauseSatisfied)
        this->mClauseSatisfied[this->mClauseSatisfied.size()]=s.second;
    for (auto& s:formule2->mClauseUnsatisfied)
        this->mClauseUnsatisfied[this->mClauseUnsatisfied.size()]=s.second;
}
