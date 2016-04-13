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


bool backtrack(){
    if(proof){
        writeAxiom();
    }
    //renvoie false si le backtrack n'as pas marché
    //-> plus de retour en arrière possible
    //rappel : assignations et instance sont globales
    int i=assignations.size()-1;
    int level_back = 0;
    while ((i>=0) && (level_back == 0)){
        if (assignations[i]->bet==false){
            if(proof){
                writeDeduce(assignations[i]);
            }
            assignations[i]->variable->value=-1;
            assignations[i]->updateStatus(true);
            if (interactive)
                assignations[i]->variable->varConflict.clear();

            if(proof){
                writeAssign(assignations[i]);
            }
            if(proof and assignations[i]->bet==0 and bets[i])
                writeBinary();
            //la deduction binaire ne se fait qu'apres avoir ramené a la vie le litteral

            delete assignations[i];
            assignations.pop_back();
            bets.pop_back();
            i--;
        }else{
            if(proof){
                writeDeduce(assignations[i]);
            }
            assignations[i]->updateStatus(true);
            //on ne change les clauses que là où les variables sont mortes !
            //i.e. les clauses sont considérées satisfaites ou non en fonctions
            //de leurs litteraux mort uniquement !
            if(proof){
                writeAssign(assignations[i]);
            }
            assignations[i]->bet=false;
            assignations[i]->variable->value=(1-assignations[i]->variable->value);
            //attention, si la variable venait à ne pas etre assignée
            //(value == -1), le backtrack fail lamentablement
            //ceci n'est pas possible en théorie, car toute variable dans
            //assignation est assignée
            assignations[i]->updateStatus(false);
            level_back++;
//            level_cur--; //TODO
        }
    }
    return (level_back != 0);
}

bool conflictAnal(clause* cl_Conflict){//Renvoie false si l'analyse de conflit ne peut plus remonter (comment?)
    clause* UIPclause = getUIPClause(cl_Conflict);
    if(interactive){//On affiche le graphe
        string command;
        cout<<"Backtrack breakpoint, enter a command: ";
        cin>>command;
        if (command!="c"){
            if (command=="g"){
//                getUIP
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
                    fprintf(graph_file,"p<SUB>%i</SUB><SUP>%i</SUP>",ass->variable->id,ass->variable->level_ass);
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
                        fprintf(graph_file,"p<SUB>%i</SUB><SUP>%i</SUP>",v2->id,v2->level_ass);
                        if (v2 == var_decided)
                            fprintf(graph_file,"<SUP>d</SUP>>];\n");
                        else
                            fprintf(graph_file,">];\n");
                        fprintf(graph_file,"%i -> %i;\n",v2->id,ass->variable->id);
                    }
                }
                //On affiche l'UIP
                for(litt* li = UIPclause->f_ElementDead;li != nullptr;li = li->next_litt){
                    fprintf(stderr,"The learned clause is :");
                    UIPclause->print();
                    if (li->variable->level_ass == level_cur)
                        fprintf(graph_file,"%i [fillcolor=yellow];\n",li->variable->id);
                    else
                        fprintf(graph_file,"%i [fillcolor=purple];\n",li->variable->id);
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
    //on procède à l'analyse de conflit
    int level_max_back = 0;
    for(litt* li = UIPclause->f_ElementDead;li != nullptr;li = li->next_litt) {
        if (li->variable->level_ass != level_cur && li->variable->level_ass > level_max_back)
            level_max_back = li->variable->level_ass;
    }
    appendClause(&(instance->f_ClauseUnsatisfied),&(instance->f_ClauseUnsatisfied),UIPclause);
    //on backtrack jusqu'à level_max_back
    /*while (level_cur > level_max_back){
        if (assignations[i]->bet==true)
            level_cur--;
        assignations[i]->variable->value=-1;
        assignations[i]->updateStatus(true);
        if (clLearning)
            assignations[i]->variable->clConflict = nullptr;
        if (interactive)
            assignations[i]->variable->varConflict.clear();
        delete assignations[i];
        assignations.pop_back();
        i--;
    }*/
    return true;
}

clause* getUIPClause(clause *cl_Conflict){
    clause* clLearned = cl_Conflict->copy();

    var* var_ref = nullptr;
    bool unique = false;
    assignation* ass = nullptr;
    bool li_need_back;
    litt* li_prev;
    vector<pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
    //vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)

    while(!unique){
        //On récupère li_ref pour faire une résolution si nécessaire
        var_ref = nullptr;
        unique = true;
        for(litt* li=clLearned->f_ElementAlive;li!=nullptr;li=li->next_litt){
            if (li->variable->level_ass == level_cur){
                if (var_ref == nullptr)
                    var_ref = li->variable;
                else {
                    if (li->variable != var_ref){//On évite d'être préoccupé par les doublons
                        unique = false;
                        for(int i = assignations.size()-1;i>=0;i--){//On conserve dans li_ref, le littéral déduit le plus tard (on fait ici l'hypothèse qu'il n'existe pas de littéraux déduits au niveau n avant le littéral décidé au niveau n. Sinon, li_ref n'est pas assuré d'être déduit et non décidé)
                            ass = assignations[i];
                            if (ass->variable == li->variable){
                                var_ref = li->variable;
                                break;
                            } else if (ass->variable == var_ref)
                                break;
                        }//on break forcément car on a au moins li_ref dans assignations
                    } else {
                        fprintf(stderr,"Warning: duplicates detected while learning clauses");
                    }
                }
            }
        }
        if (!unique){//on va faire une résolution via li_ref
            if (var_ref->clConflict == nullptr)
                fprintf(stderr,"Fatal: Attempt to build learned clause via a variable deduced by no direct clause\n");
            clLearned->merge(var_ref->clConflict->copy());
            //On enlève les doublons et les occurences à li_ref ou sa négation
            li_need_back = false;
            li_prev = nullptr;
            for (litt* li = clLearned->f_ElementDead;li != nullptr || li_need_back;li = li->next_litt) {
                if (li_need_back){
                    li=li_prev;
                    li_prev=nullptr;
                    li_need_back = false;
                }
                if (li->neg){
                    if ((variables[li->variable->id].first > 0) || (li->variable == var_ref)){//si on a un doublon dans la clause ou var_ref, on l'élimine
                        removeLitt(&clLearned->f_ElementDead,&clLearned->l_ElementDead,li,li_prev);
                        delete li;
                        if (li_prev != nullptr)
                            li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (clLearned->f_ElementDead != nullptr){
                            li = clLearned->f_ElementDead;
                            li_need_back = true;
                        } else//there is nothing left
                            break;
                    } else {
                        variables[li->variable->id].first++;
                    }
                } else {
                    if (variables[li->variable->id].second > 0 || (li->variable == var_ref)){//si on a un doublon dans la clause ou var_ref, on l'élimine
                        removeLitt(&clLearned->f_ElementDead,&clLearned->l_ElementDead,li,li_prev);
                        delete li;
                        if (li_prev != nullptr)
                            li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (clLearned->f_ElementDead != nullptr){
                            li = clLearned->f_ElementDead;
                            li_need_back = true;
                        } else//there is nothing left
                            break;
                    } else {
                        variables[li->variable->id].second++;
                    }
                }
                li_prev = li;
            }
            for (auto& v:variables){
                if (v.first != 0 and v.second != 0){//la clause est tautologique
                    fprintf(stderr,"Warning: the learning clause is tautological !\n");
                }
                v.first = 0;
                v.second = 0;
            }
        }
    }
    return clLearned;
}


