#include "backtrack.h"


clause* getConflict(){//renvoie (si elle existe) un pointeur vers une clause insatisfaite qui n'a plus de littéraux vivants (formule insatisfiable), ou nullptr sinon.
	for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause)
        if (cl->nbLittAlive()==0)
            return cl;
//    for (auto& s:instance->mClauseUnsatisfied){
//        if (s.second != nullptr and s.second->nbLittAlive()==0){
//            return false;
//        }
//    }
    return nullptr;
}


bool backtrack(clause* cl_Conflict){
    //renvoie false si le backtrack n'as pas marché
    //-> plus de retour en arrière possible
    //rappel : assignations et instance sont globales
    if(interactive){
        string command;
        cin>>command;
        if (command!="c\n"){
            if (command=="g"){
                fprintf(stderr,"digraph conflict {\nnode [style=\"filled,rounded\",shape=circle,fillcolor=white];\n");
                for(assignation* ass:assignations){
                    //On affiche la variable du niveau courant
                    fprintf(stderr,"%i [label=<",ass->variable->id);
                    if (ass->variable->value == 0)
                        fprintf(stderr,"¬");
                    fprintf(stderr,"p<SUB>%i</SUB>",ass->variable->id);
                    if (ass->bet == 1)
                        fprintf(stderr,"<SUP>d</SUP>");
                    fprintf(stderr,">,fillcolor=lightblue];\n");
                    //On l'affiche alors en bleu
                    for(var* v2:ass->variable->varConflict){
                        //On affiche la variable à l'origine de la déduction
                        fprintf(stderr,"%i [label=<",v2->id);
                        if (v2->value == 0)
                            fprintf(stderr,"¬");
                        fprintf(stderr,"p<SUB>%i</SUB>>];\n",v2->id);
                        fprintf(stderr,"%i -> %i;\n",v2->id,ass->variable->id);
                    }
                }
                //On affiche le conflict
                fprintf(stderr,"conflict [fillcolor=lightred];\n");
                //On affiche les liaisons avec les variables responsables du conflit
                for(litt* li = cl_Conflict->f_ElementDead;li != nullptr;li = li->next_litt)
                    fprintf(stderr,"%i -> conflict;\n",li->variable->id);
                fprintf(stderr,"}\n");
                //TODO : discuss data structure implementation
                //TODO : create graph !
            }else if(command=="t\n"){
                interactive=false;
            }else
                cerr<<"No valid command found, continuing anyway"<<endl;
        }
    }
    int i=assignations.size()-1;
    bool hasChanged=false;
    while (i>=0){
        if (assignations[i]->bet==false){
            assignations[i]->variable->value=-1;
            assignations[i]->updateStatus(true);
            assignations[i]->variable->varConflict.clear();
            delete assignations[i];
            assignations.pop_back();
            i--;
        }else{
            assignations[i]->updateStatus(true);
            //on ne change les clauses que là où les variables sont mortes !
            //i.e. les clauses sont consiférées satisfaites ou non en fonctions
            //de leurs litteraux mort uniquement !
            assignations[i]->bet=false;
            assignations[i]->variable->value=(1-assignations[i]->variable->value);
            //attention, si la variable venait à ne pas etre assignée
            //(value == -1), le backtrack fail lamentablement
            //ceci n'est pas possible en théorie, car toute variable dans
            //assignation est assignée
            assignations[i]->updateStatus(false);
            hasChanged=true;
            break;
        }
    }
    return hasChanged;
}
