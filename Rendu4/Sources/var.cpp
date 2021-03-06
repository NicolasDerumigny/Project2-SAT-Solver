#include "var.h"


void var::print(){
    //affiche juste l'affectation, pas d'espace ni de std::endl
    switch (this->value){
        case 1:
            std::cout<<this->id;
            break;

        case 0:
            std::cout<<"-"<<this->id;
            break;

       case -1:
            std::cout<<"±"<<this->id;
            break;
   }
}


void var::set_var(unsigned long id_enter){
    this->id=id_enter;
    this->value=-1;
}

void var::assignValue(int value_enter ,bool bet_enter) {
    this->value=value_enter;
    this->bet=bet_enter;
    if (clLearning){
        this->level_ass = level_cur;
    }
//On met à jour les variables mortes/vivantes, vis-à-vis du passage de variable de vivant à mort.
//	this->updateStatus(false);
//On ajoute cette assignation dans la pile d'assignations en précisant qu'il s'agit d'un pari.
	assignation* new_assign;
	new_assign = new assignation;
    new_assign->set_assign(this,bet_enter);
    new_assign->updateStatus(false);
	assignations.push_back(new_assign);
}

void set_var_tseitin(var ** new_var){
    *new_var = new var;
    (*new_var)->set_var(v_var_tseitin.size());
    v_var_tseitin.push_back(*new_var);
}
