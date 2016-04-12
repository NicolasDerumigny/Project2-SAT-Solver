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
        cout<<"Backtrack breakpoint, enter a command: ";
        cin>>command;
        if (command!="c"){
            if (command=="g"){
                FILE* graph_file = fopen("./graph.dot","w");
                if (graph_file == nullptr){
                    perror("Warning: Unable to write conflict graph");
                    fprintf(stderr,"Outputting graph on stderr\n");
                    graph_file = stderr;
                }
                var* var_decided = nullptr;
                fprintf(graph_file,"digraph conflict {\nnode [style=\"filled,rounded\",shape=circle,fillcolor=white];\n");
                for(assignation* ass:assignations){
                    //On affiche la variable du niveau courant
                    fprintf(graph_file,"%i [label=<",ass->variable->id);
                    if (ass->variable->value == 0)
                        fprintf(graph_file,"¬");
                    fprintf(graph_file,"p<SUB>%i</SUB>",ass->variable->id);
                    if (ass->bet == 1){
                        fprintf(graph_file,"<SUP>d</SUP>");
                        var_decided = ass->variable;
                    }
                    fprintf(graph_file,">,fillcolor=lightblue];\n");
                    //On l'affiche alors en bleu
                    for(var* v2:ass->variable->varConflict){
                        //On affiche la variable à l'origine de la déduction
                        fprintf(graph_file,"%i [label=<",v2->id);
                        if (v2->value == 0)
                            fprintf(graph_file,"¬");
                        fprintf(graph_file,"p<SUB>%i</SUB>",v2->id);
                        if (v2 == var_decided)
                            fprintf(graph_file,"<SUP>d</SUP>>];\n");
                        else
                            fprintf(graph_file,">];\n");
                        fprintf(graph_file,"%i -> %i;\n",v2->id,ass->variable->id);
                    }
                }
                //On affiche le conflict
                fprintf(graph_file,"conflict [fillcolor=red];\n");
                //On affiche les liaisons avec les variables responsables du conflit
                for(litt* li = cl_Conflict->f_ElementDead;li != nullptr;li = li->next_litt)
                    fprintf(graph_file,"%i -> conflict;\n",li->variable->id);
                fprintf(graph_file,"}\n");
                if (fclose(graph_file) != 0)
                    perror("Warning: Unable to close the conflict graph file, possible I/O errors incoming");
                //TODO : discuss data structure implementation
                //TODO : create graph !
                if (graph_file != stderr){
                    /*int val_ret = fork();
                    if (val_ret == 0){//On est dans le fils
                            execvp("dot","dot -Tps:cairo graph.dot -o graph.ps");
                            exit(0);
                    } else if (val_ret > 0){//On est dans le père
                        waitpid(val_ret,nullptr,0);
                    } else {
                        perror("Warning: unable to convert graph");
                    }*/
                    if (system("dot -Tps:cairo graph.dot -o graph.ps") != 0)
                        fprintf(stderr,"Warning: unable to convert conflict graph. Maybe dot is not installed\nTo solve it, run: sudo apt-get install graphviz\n");
                    else
                        if (system("evince graph.ps 2> /dev/null &") != 0)
                            fprintf(stderr,"Warning: unable to display graph. Maybe evince is not installed\nTo solve it, run: sudo apt-get install evince\n");
                }
            }else if(command=="t"){
                interactive=false;
            }else
                cerr<<"No valid command found, continuing anyway"<<endl;
        }
    }
    int i=assignations.size()-1;
    int level_back = 0;
    while ((i>=0) && (level_back == 0)){
        if (assignations[i]->bet==false){
            assignations[i]->variable->value=-1;
            assignations[i]->updateStatus(true);
            if (interactive)
                assignations[i]->variable->varConflict.clear();
            delete assignations[i];
            assignations.pop_back();
            i--;
        }else{
            assignations[i]->updateStatus(true);
            //on ne change les clauses que là où les variables sont mortes !
            //i.e. les clauses sont considérées satisfaites ou non en fonctions
            //de leurs litteraux mort uniquement !
            assignations[i]->bet=false;
            assignations[i]->variable->value=(1-assignations[i]->variable->value);
            //attention, si la variable venait à ne pas etre assignée
            //(value == -1), le backtrack fail lamentablement
            //ceci n'est pas possible en théorie, car toute variable dans
            //assignation est assignée
            assignations[i]->updateStatus(false);
            level_back++;
        }
    }
    return (level_back != 0);
}
