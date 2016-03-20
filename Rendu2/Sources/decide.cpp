#include <vector>
#include "../Header/assignation.h"
#include <stdlib.h>
#include <time.h>

var* getFreeVar() {
	for (var* s:v_var) {
        if (s != nullptr && s->value == -1) {
			return s;
		}
	}
	return nullptr;
}

var* getRandFreeVar() {
	srand(time(NULL));
	var* new_var = nullptr;
	bool no_more = true;
	for (var* s:v_var) {
		if (s != nullptr && s->value == -1) {
			no_more = false;
			break;
		}
	}
	if (no_more)
		return nullptr;
	unsigned int i_var;
	while (new_var == nullptr) {
		i_var = rand() % v_var.size();
		if (v_var[i_var] != nullptr && v_var[i_var]->value == -1) {
			new_var = v_var[i_var];
		}
	}
	return new_var;
}

var* getMomsFreeVar() {
	int cl_size;
	if (instance->f_ClauseUnsatisfied = nullptr)//Si toutes les clauses sont déjà satisfaites, on renvoie la première variable encore vivante qu'on voit
		return getFreeVar();
	int clause_min_size = instance->f_ClauseUnsatisfied->nbLittAlive();//Sinon on récupère la taille minimale des clauses satisfaites
	for (clause* cl = instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause) {
		cl_size = cl->nbLittAlive();
		if (cl_size < clause_min_size)
			clause_min_size = cl_size;
	}	
	int occ_max = 0;
	int occ_sum = 0;
	var* new_var = nullptr;
	for (auto& v:v_var){//puis pour chaque variable, on stocke dans occ_sum le nombre de fois où elle apparait dans une clause de taille minimale.
		if (v != nullptr && v->value == -1) {
			occ_sum = 0;
			for (auto& cl:v->clauseInto)
				if (cl->nbLittAlive() == clause_min_size)
					occ_sum++;
			if (occ_sum > occ_max) {//on met à jour la variable qui apparait le plus (nb_occ)
				occ_max = occ_sum;
				new_var = v;
			}
		}
	}
	return new_var;
}

//voir var::assignValue avec value=1 par défaut et bet=true
