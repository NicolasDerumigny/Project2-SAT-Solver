#include "backtrack.h"


bool checkAssign(){
    std::map<unsigned long,union_set<unsigned long> > valToSet;
    unsigned long left_nb;
    unsigned long right_nb;
    for (assignation* ass:assignations){
        if (ass->variable != nullptr){//on crée des sets pour chaque élément de la théorie rencontré
            left_nb = std::get<0>(varToEq[ass->variable->id]);
            right_nb = std::get<1>(varToEq[ass->variable->id]);
            if (!valToSet.count(left_nb)){
                union_set<unsigned long> left (left_nb);
                valToSet[left_nb] = left;
            }
            if (!valToSet.count(right_nb)){
                union_set<unsigned long> right (right_nb);
                valToSet[right_nb] = right;
            }
        }
    }
    for (assignation* ass:assignations){//si deux éléments sont égaux, on fusionne les sets
        if ((ass->variable->value == 1 && std::get<2>(varToEq[ass->variable->id])) || (ass->variable->value == 0 && !std::get<2>(varToEq[ass->variable->id]))){
            left_nb = std::get<0>(varToEq[ass->variable->id]);
            right_nb = std::get<1>(varToEq[ass->variable->id]);
            merge(valToSet[left_nb],valToSet[right_nb]);
        }
    }
    for (assignation* ass:assignations){
        if ((ass->variable->value == 0 && std::get<2>(varToEq[ass->variable->id])) || (ass->variable->value == 1 && !std::get<2>(varToEq[ass->variable->id]))){
            left_nb = std::get<0>(varToEq[ass->variable->id]);
            right_nb = std::get<1>(varToEq[ass->variable->id]);
            if (valToSet[left_nb].find() == valToSet[right_nb].find()){//Alors c'est insatisfiable dans la théorie
                clause* new_clause = new clause;
                new_clause->set_clause(nullptr);
                for (assignation* ass:assignations){
                    litt* new_litt = new litt;
                    new_litt->set_litt(ass->variable,(ass->variable->value == 1));
                    appendLitt(&new_clause->f_ElementDead,&new_clause->l_ElementDead,new_litt);
                }
                appendClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,new_clause);
                return false;
            }
        }
    }
    return true;
}

clause* getConflict(){//renvoie (si elle existe) un pointeur vers une clause insatisfaite qui n'a plus de littéraux vivants (formule insatisfiable), ou nullptr sinon.
	for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr;cl=cl->next_clause)
        if (cl->nbLittAlive()==0)
            return cl;
    return nullptr;
}


bool backtrack(){
    if(proof){
        writeAxiom();
    }
    //renvoie false si le backtrack n'as pas marché
    //-> plus de retour en arrière possible
    //rappel : assignations et instance sont globales
    unsigned long i=assignations.size()-1;
    unsigned long level_back = 0;
    while (level_back == 0){
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
            if (i==0)
                break;
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
            assignations[i]->variable->bet=false;
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
    if (level_cur == 0 && !interactive)//on a un conflit qui n'est pas lié à des paris
        return false;//alors la formule est unsatisfiable
    clause* UIPclause=nullptr;
    if (level_cur != 0)
        UIPclause = getUIPClause(cl_Conflict);
    if(interactive){//On affiche le graphe
        std::string command;
        std::cout<<"Backtrack breakpoint, enter a command: ";
        std::cin>>command;
        if (command!="c"){
            if (command=="g"){
//                getUIP
                FILE* graph_file = fopen("./graph.dot","w");
                if (graph_file == nullptr){
                    perror("Warning: Unable to write conflict graph");
                    fprintf(stderr,"Outputting graph on stderr\n");
                    graph_file = stderr;
                }
                fprintf(graph_file,"digraph conflict {\nnode [style=\"filled,rounded\",shape=circle,fillcolor=white];\n");
                for(assignation* ass:assignations){
                    if (ass->variable->level_ass == level_cur){//On affiche la variable du niveau courant
                        fprintf(graph_file,"%lu [label=<",ass->variable->id);
                        if (ass->variable->value == 0)
                            fprintf(graph_file,"¬");
                        fprintf(graph_file,"p<SUB>%lu</SUB><SUP>%i</SUP>",ass->variable->id,ass->variable->level_ass);
                        if (ass->bet)
                            fprintf(graph_file,"<SUP>d</SUP>");
                        if (ass->variable->UIP){//On l'affiche alors en bleu ou en violet si il appartient à l'ensemble UIP
                            ass->variable->UIP = false;
                            fprintf(graph_file,">,fillcolor=purple];\n");
                        } else
                            fprintf(graph_file,">,fillcolor=lightblue];\n");
                        for(var* v2:ass->variable->varConflict){
                            //On affiche les variables à l'origine de la déduction
                            fprintf(graph_file,"%lu [label=<",v2->id);
                            if (v2->value == 0)
                                fprintf(graph_file,"¬");
                            fprintf(graph_file,"p<SUB>%lu</SUB><SUP>%i</SUP>",v2->id,v2->level_ass);
                            if (v2->bet)
                                fprintf(graph_file,"<SUP>d</SUP>>];\n");
                            else
                                fprintf(graph_file,">];\n");
                            fprintf(graph_file,"%lu -> %lu;\n",v2->id,ass->variable->id);
                        }
                    }
                }
                //On affiche l'UIP
                if (level_cur != 0){
                    fprintf(stderr,"The learned clause is :");
                    UIPclause->print();
                    for(litt* li = UIPclause->f_ElementDead;li != nullptr;li = li->next_litt){
                        if (li->variable->level_ass == level_cur)
                            fprintf(graph_file,"%lu [fillcolor=yellow];\n",li->variable->id);
//                        else
//                            fprintf(graph_file,"%lu [fillcolor=purple];\n",li->variable->id);
                    }
                }
                //On affiche le conflict
                fprintf(graph_file,"conflict [fillcolor=red];\n");
                //On affiche les liaisons avec les variables responsables du conflit
                for(litt* li = cl_Conflict->f_ElementDead;li != nullptr;li = li->next_litt){
                    fprintf(graph_file,"%lu [label=<",li->variable->id);
                    if (li->variable->value == 0)
                        fprintf(graph_file,"¬");
                    fprintf(graph_file,"p<SUB>%lu</SUB><SUP>%i</SUP>",li->variable->id,li->variable->level_ass);
                    if (li->variable->bet)
                        fprintf(graph_file,"<SUP>d</SUP>>];\n");
                    else
                        fprintf(graph_file,">];\n");
                    fprintf(graph_file,"%lu -> conflict;\n",li->variable->id);
                }
                fprintf(graph_file,"}\n");
                if (fclose(graph_file) != 0)
                    perror("Warning: Unable to close the conflict graph file, possible I/O errors incoming");
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
                std::cerr<<"No valid command found, continuing anyway"<<std::endl;
        }
    }
    if (level_cur == 0)
        return false;
    //on procède à l'analyse de conflit
    int level_max_back = 0;
    for(litt* li = UIPclause->f_ElementDead;li != nullptr;li = li->next_litt) {
        if (heuristic == 4)
            li->variable->score+=32;
        if (li->variable->level_ass != level_cur && li->variable->level_ass > level_max_back)
            level_max_back = li->variable->level_ass;
    }
    if (heuristic == 4)
        for(auto& v:v_var){
            if (v != nullptr)
               v->score = v->score / 2;
        }
    appendClause(&(instance->f_ClauseUnsatisfied),&(instance->l_ClauseUnsatisfied),UIPclause);
    //on backtrack jusqu'à level_max_back
    unsigned long i = assignations.size()-1;
    while (level_cur > level_max_back){
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
        if(i==0)
            break;
        i--;
    }

    if (clHeuristic == 1){//on applique l'heuristique forget juste après l'analyse de conflit, car on ne veut pas s'encombrer de clauses inutiles pour le deduce qui vient, mais on veut avoir plus de chance que les clauses à supprimer ne soient pas déjà utilisées à ce niveau
        UIPclause->score += 32;
        bool isRemovable = true;
        bool cl_need_back = false;
        clause* cl_prev = nullptr;
        for (clause* cl=instance->f_ClauseUnsatisfied;cl != nullptr || cl_need_back;cl=cl->next_clause){
            if (cl_need_back){
                cl=cl_prev;
                cl_prev=nullptr;
                cl_need_back = false;
            }
            if (cl->isLearned){
                cl->score = cl->score / 2;
                if (cl->score <= 1){
                    isRemovable = true;
                    for (var* v:v_var){//on vérifie que cette clause n'est pas responsable de l'assignation d'une variable au niveau courant
                        if (v != nullptr){
                            if (v->clConflict == cl){
                                isRemovable = false;
                            }
                        }
                    }
                    if (isRemovable){
                        removeClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,cl);
                        for (litt* li = cl->f_ElementAlive;li != nullptr;li = li->next_litt){//supprimer la clause cl des clauseInto
                            li->variable->clauseInto.erase(std::remove(li->variable->clauseInto.begin(), li->variable->clauseInto.end(), cl), li->variable->clauseInto.end());
                        }
                        for (litt* li = cl->f_ElementDead;li != nullptr;li = li->next_litt){//supprimer la clause cl des clauseInto
                            li->variable->clauseInto.erase(std::remove(li->variable->clauseInto.begin(), li->variable->clauseInto.end(), cl), li->variable->clauseInto.end());
                        }
                        cl->free_clause();
                        delete cl;
                        if (cl_prev != nullptr)
                            cl = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (instance->f_ClauseUnsatisfied != nullptr){
                            cl = instance->f_ClauseUnsatisfied;
                            cl_need_back = true;
                        } else//there is nothing left
                            break;
                    }
                }
            }
            cl_prev = cl;
        }
        cl_need_back = false;
        cl_prev = nullptr;
        for (clause* cl=instance->f_ClauseSatisfied;cl != nullptr || cl_need_back;cl=cl->next_clause){
            if (cl_need_back){
                cl=cl_prev;
                cl_prev=nullptr;
                cl_need_back = false;
            }
            if (cl->isLearned){
                cl->score = cl->score / 2;
                if (cl->score <= 1){
                    isRemovable = true;
                    for (var* v:v_var){//on vérifie que cette clause n'est pas responsable de l'assignation d'une variable au niveau courant
                        if (v != nullptr){
                            if (v->clConflict == cl){
                                isRemovable = false;
                            }
                        }
                    }
                    if (isRemovable){
                        removeClause(&instance->f_ClauseSatisfied,&instance->l_ClauseSatisfied,cl);
                        for (litt* li = cl->f_ElementAlive;li != nullptr;li = li->next_litt){//supprimer la clause cl des clauseInto
                            li->variable->clauseInto.erase(std::remove(li->variable->clauseInto.begin(), li->variable->clauseInto.end(), cl), li->variable->clauseInto.end());
                        }
                        for (litt* li = cl->f_ElementDead;li != nullptr;li = li->next_litt){//supprimer la clause cl des clauseInto
                            li->variable->clauseInto.erase(std::remove(li->variable->clauseInto.begin(), li->variable->clauseInto.end(), cl), li->variable->clauseInto.end());
                        }
                        cl->free_clause();
                        delete cl;
                        if (cl_prev != nullptr)
                            cl = cl_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (instance->f_ClauseUnsatisfied != nullptr){
                            cl = instance->f_ClauseUnsatisfied;
                            cl_need_back = true;
                        } else//there is nothing left
                            break;
                    }
                }
            }
            cl_prev = cl;
        }
    }

    return true;
}

clause* getUIPClause(clause *cl_Conflict){
    clause* clLearned = cl_Conflict->copy();
    if (clHeuristic == 1){
        cl_Conflict->score += 32;
    }

    var* var_ref = nullptr;
    bool unique = false;
    assignation* ass = nullptr;
    bool li_need_back;
    litt* li_prev;
    std::vector<std::pair<int,int> > variables (v_var.size(), std::make_pair(0,0));
    //vector contenant pour chaque variable une paire (nb_fois_vue_niée,nb_fois_vue_non_niée)

    while(!unique){
        //On récupère var_ref pour faire une résolution si nécessaire
        var_ref = nullptr;
        unique = true;
        for(litt* li=clLearned->f_ElementDead;li!=nullptr;li=li->next_litt){
            if (li->variable->level_ass == level_cur){
                if (var_ref == nullptr)
                    var_ref = li->variable;
                else {
                    if (li->variable != var_ref){//On évite d'être préoccupé par les doublons
                        unique = false;
                        for(unsigned long i = assignations.size()-1;;i--){
                            //On conserve dans var_ref, le littéral déduit le plus tard (on fait ici l'hypothèse qu'il n'existe pas de littéraux déduits au niveau n avant le littéral décidé au niveau n. Sinon, var_ref n'est pas assuré d'être déduit et non décidé)
                            ass = assignations[i];
                            if (ass->variable == li->variable){
                                var_ref = li->variable;
                                break;
                            } else if (ass->variable == var_ref)
                                break;
                            if (i==0)
                                break;
                        }//on break forcément car on a au moins var_ref dans assignations
                    } else {
                        fprintf(stderr,"Warning: duplicates detected while learning clauses\n");
                    }
                }
            }
        }
        if (!unique){//on va faire une résolution via var_ref
            if (interactive)
                var_ref->UIP=true;
            if (var_ref->clConflict == nullptr)
                fprintf(stderr,"Fatal: Attempt to build learned clause via a variable deduced by no direct clause\n");
//            clLearned->print();var_ref->clConflict->print();cl_Conflict->print();printf("%p,%p\n",clLearned,cl_Conflict);
            if (clHeuristic == 1){
                var_ref->clConflict->score += 32;
            }
//            fprintf(stderr,"copy of clause %p\n",var_ref->clConflict);
            clLearned->merge(var_ref->clConflict->copy());
//            clLearned->print();var_ref->clConflict->print();cl_Conflict->print();printf("%p,%p\n",clLearned,cl_Conflict);
            //On enlève les doublons et les occurences des littéraux associés à var_ref ou leur négation
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
                        removeLitt(&clLearned->f_ElementDead,&clLearned->l_ElementDead,li);
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
                        removeLitt(&clLearned->f_ElementDead,&clLearned->l_ElementDead,li);
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
            //on met à jour les clauseInto (clLearned éliminé pour var_ref, et suppression des doublons)
            var_ref->clauseInto.erase(std::remove(var_ref->clauseInto.begin(), var_ref->clauseInto.end(), clLearned), var_ref->clauseInto.end());
            for(litt* li = clLearned->f_ElementDead;li != nullptr || li_need_back;li = li->next_litt){
                sort(li->variable->clauseInto.begin(), li->variable->clauseInto.end());
                li->variable->clauseInto.erase(std::unique(li->variable->clauseInto.begin(), li->variable->clauseInto.end()), li->variable->clauseInto.end());
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

    clLearned->isLearned=true;

    if(wl){
         bool found1=false, found2=false;
         clLearned->w_litt_1=getFirstTrue(clLearned->f_ElementDead);
         found1=(clLearned->w_litt_1!=nullptr);
         clLearned->w_litt_2=getFirstTrue(clLearned->w_litt_1->next_litt);
         found2=(clLearned->w_litt_2!=nullptr);

         if(found1 and !found2){
             if (clLearned->w_litt_1->prev_litt!=nullptr)
                 clLearned->w_litt_2=clLearned->w_litt_1->prev_litt;
             else
                 clLearned->w_litt_2=clLearned->w_litt_1->next_litt;
             found2=clLearned->w_litt_2!=nullptr;
         }

         if(!found1){
             clLearned->w_litt_1=clLearned->f_ElementDead;
             found1=clLearned->w_litt_1!=nullptr;
             clLearned->w_litt_2=clLearned->l_ElementDead;
             found2=clLearned->w_litt_2!=nullptr and clLearned->w_litt_2!=clLearned->w_litt_1;
         }



         if(!found1){
             clLearned->w_litt_1=clLearned->f_ElementAlive;
             found1=clLearned->w_litt_1!=nullptr;
             clLearned->w_litt_2=clLearned->l_ElementAlive;
             found2=clLearned->w_litt_2!=nullptr and clLearned->w_litt_2!=clLearned->w_litt_1;
         }else if(!found2){
             clLearned->w_litt_2=clLearned->f_ElementAlive;
             found2=clLearned->w_litt_2!=nullptr and clLearned->w_litt_2!=clLearned->w_litt_1;
         }

         if(!found2){
            std::cerr<<"Error : can't apply watched litterals method (one clause learned has only one variable) : returning to normal state"<<std::endl;
            wl=false;
         }
    }

    return clLearned;
}


