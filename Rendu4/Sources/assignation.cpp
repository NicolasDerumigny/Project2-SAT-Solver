#include "assignation.h"


void assignation::set_assign(var* variable_enter, bool bet_enter) {
    bets.push_back(bet_enter);
    this->variable=variable_enter;
    this->bet=bet_enter;
}

sem_t sem_clauseInto;

void* updateLitt_t(void* arg){
    bool li_need_back = false;
    litt* li_prev = nullptr;
    bool alive = ((arg_updateLitt*) arg)->alive;
    var* arg_variable = ((arg_updateLitt*) arg)->variable;
//    sem_t sem_clauseInto = ((arg_updateLitt*) arg)->sem_clauseInto;
    int val;
    while(true){
        if (sem_wait(&sem_clauseInto) != 0){perror("sem_wait: ");}//On entre dans la section critique
        sem_getvalue(&sem_clauseInto,&val);
//        fprintf(stderr,"entering SC, sem=%i\n",val);
        if (index_t >= ((arg_updateLitt*) arg)->variable->clauseInto.size()){
//            fprintf(stderr,"index_t is full\n");
            if (sem_post(&sem_clauseInto) != 0){perror("sem_post: ");}
            break;
        }
        clause* cl = ((arg_updateLitt*) arg)->variable->clauseInto[index_t];
//        fprintf(stderr,"using %i\n",index_t);
        index_t++;
        if (sem_post(&sem_clauseInto) != 0){perror("sem_post: ");}
        if (alive == false) { //si on tue une variable, on recherche les littéraux associés dans les éléments vivants et on les transfères vers les morts.
            li_need_back = false;
            li_prev = nullptr;
            for (litt* li = cl->f_ElementAlive;li != nullptr or li_need_back;li=li->next_litt){
                //si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li_need_back){
                    li=li_prev;
                    li_prev=nullptr;
                    li_need_back = false;
                }
                if (li->variable == arg_variable) {
                    removeLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li);
                    appendLitt(&cl->f_ElementDead,&cl->l_ElementDead,li);
                    if (needNewWatched(cl,li) and !li->isTrue())
                        assignNewWatched(cl,li);
					if (li_prev != nullptr)
						li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
					else if (cl->f_ElementAlive != nullptr){//on est au début
						li = cl->f_ElementAlive;
                        li_need_back = true;
					} else//there is nothing left
                        break;
                }
                //renvoie false si on n'a pas trouvé d'autre litteral possible
                //cependant, si rien n'a été trouvé, on fera bien le backtrack
                li_prev = li;
            }
        } else { //et réciproquement...
            li_need_back = false;
            li_prev = nullptr;
            for (litt* li = cl->f_ElementDead;li != nullptr || li_need_back;li=li->next_litt) {//si un littéral (donc la variable) est déjà mort on ne fait rien.
                if (li_need_back){
                    li=li_prev;
                    li_prev=nullptr;
                    li_need_back = false;
                }
                if (li->variable == arg_variable) {
                    removeLitt(&cl->f_ElementDead,&cl->l_ElementDead,li);
                    appendLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li);
                    if (li_prev != nullptr)
                        li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
                    else if (cl->f_ElementDead != nullptr) {
                        li = cl->f_ElementDead;
                        li_need_back = true;
                    } else//there is nothing left
                        break;
                }
                li_prev = li;
            }
        }
    }
    pthread_exit(nullptr);
}

void assignation::updateLitt(bool alive){
    bool threadsWorked = false;
//    fprintf(stderr,"threading %lu\n",this->variable->clauseInto.size());
    if (nb_threads != 0){
//        fprintf(stderr,"threading\n");
        if (sem_init(&sem_clauseInto,0,1) != 0){perror("sem_init: ");};
        arg_updateLitt arg;
        arg.alive=alive;
//        arg.sem_clauseInto=sem_clauseInto;
        arg.variable=this->variable;
        index_t = 0;
        nb_threads = NB_THREADS;
        pthread_t thread_workers[NB_THREADS];
        for (int i=0;i<nb_threads;i++){
//            fprintf(stderr,"creating thread %i\n",i);
            if (pthread_create(&thread_workers[i],nullptr,updateLitt_t,&arg) != 0){perror("pthread_create: ");}
            else{
//                fprintf(stderr,"created thread %i\n",i);
                threadsWorked = true;
            }
        }
        for (int i=0;i<nb_threads;i++){
            if (pthread_join(thread_workers[i],nullptr) != 0){perror("pthread_join: ");}
        }
        if (sem_destroy(&sem_clauseInto) != 0){perror("sem_destroy: ");}
    }
    if (nb_threads == 0 || !threadsWorked){
        bool li_need_back = false;
        litt* li_prev = nullptr;
        for (auto& cl:this->variable->clauseInto){
            if (alive == false) { //si on tue une variable, on recherche les littéraux associés dans les éléments vivants et on les transfères vers les morts.
                li_need_back = false;
                li_prev = nullptr;
                for (litt* li = cl->f_ElementAlive;li != nullptr or li_need_back;li=li->next_litt){
                    //si un littéral (donc la variable) est déjà mort on ne fait rien.
                    if (li_need_back){
                        li=li_prev;
                        li_prev=nullptr;
                        li_need_back = false;
                    }
                    if (li->variable == this->variable) {
                        removeLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li);
                        appendLitt(&cl->f_ElementDead,&cl->l_ElementDead,li);
                        if (needNewWatched(cl,li))
                            assignNewWatched(cl,li);
                        if (li_prev != nullptr)
                            li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (cl->f_ElementAlive != nullptr){//on est au début
                            li = cl->f_ElementAlive;
                            li_need_back = true;
                        } else//there is nothing left
                            break;
                    }
                    //renvoie false si on n'a pas trouvé d'autre litteral possible
                    //cependant, si rien n'a été trouvé, on fera bien le backtrack
                    li_prev = li;
                }
            } else { //et réciproquement...
                li_need_back = false;
                li_prev = nullptr;
                for (litt* li = cl->f_ElementDead;li != nullptr || li_need_back;li=li->next_litt) {//si un littéral (donc la variable) est déjà mort on ne fait rien.
                    if (li_need_back){
                        li=li_prev;
                        li_prev=nullptr;
                        li_need_back = false;
                    }
                    if (li->variable == this->variable) {
                        removeLitt(&cl->f_ElementDead,&cl->l_ElementDead,li);
                        appendLitt(&cl->f_ElementAlive,&cl->l_ElementAlive,li);
                        if (li_prev != nullptr)
                            li = li_prev;//On évite de casser la chaîne de parcours de la boucle for...
                        else if (cl->f_ElementDead != nullptr) {
                            li = cl->f_ElementDead;
                            li_need_back = true;
                        } else//there is nothing left
                            break;
                    }
                    li_prev = li;
                }
            }
        }
    }
}

void assignation::updateClause(bool alive){
    for (auto& cl:this->variable->clauseInto){
        if (alive == false or wl) {
            if (!cl->satisfied && cl->isSatisfied()){//on enlève cl de la liste simplement chainée des clauses non satisfaites, puis on l'ajoute aux clauses satisfaites
                removeClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,cl);
                appendClause(&instance->f_ClauseSatisfied,&instance->l_ClauseSatisfied,cl);
            }
        } if(alive == true or wl) {
            if (cl->satisfied && !cl->isSatisfied()){//on enlève cl de la liste simplement chainée des clauses satisfaites, puis on l'ajoute aux clauses non satisfaites
                removeClause(&instance->f_ClauseSatisfied,&instance->l_ClauseSatisfied,cl);
                appendClause(&instance->f_ClauseUnsatisfied,&instance->l_ClauseUnsatisfied,cl);
            }
            if (wl && cl->existsWatchedNonAlive() && cl->isSatisfied())
                std::cerr<<"Error : There exist false watched litterals in an alive clause"<<std::endl;
        }
        cl->satisfied = cl->isSatisfied();
    }
}

void assignation::updateStatus(bool alive){
    this->updateLitt(alive);
    this->updateClause(alive);
}


void assignation::print(){
    this->variable->print();
}

bool needNewWatched(clause * cl, litt* li){
    if (cl->w_litt_1==li or cl->w_litt_2==li)
        return true;
    else
        return false;
}

bool assignNewWatched(clause * cl, litt* li){
    bool changed=false;
    if (cl->w_litt_1==li){
        cl->w_litt_1=cl->w_litt_2;
        cl->w_litt_2=li;
        //li est maintenant le litteral watched numero 2
    }

    for(litt* li2=cl->f_ElementAlive;li2!=nullptr; li2=li2->next_litt){
        if (li2!=cl->w_litt_1 and li2!=li){
            cl->w_litt_2=li2;
            changed=true;
            break;
        }
    }
    return changed;
}
