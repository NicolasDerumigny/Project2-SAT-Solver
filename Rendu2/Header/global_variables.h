#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

class assignation;
class var;
class clause;
class litt;
class formule;


vector<assignation*> assignations;
//création du vector qui contient les assignations réalisées

vector<var*> v_var;
//création du vector qui contiendra tous les pointeurs vers les variables

formule * instance;
//une formule pour les gouverner tous



#endif // GLOBAL_VARIABLES_H
