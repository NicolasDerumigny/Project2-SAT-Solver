using namespace std;

bool check(){
    for (auto& s:instance->mClauseUnsatisfied){
        if (s.second != nullptr and s.second->nbLittAlive()==0){
            return false;
        }
    }
    return true;
}


bool backtrack(){
    //renvoie false si le backtrack n'as pas marché
    //-> plus de retour en arrière possible
    //rappel : assignation et instances sont globales
    int i=assignations.size()-1;
    bool hasChanged=false;
    while (i>=0){
        if (assignations[i]->bet==false){
            assignations[i]->reverseStatus();//TODO
            delete assignations[i];
            assignations.pop_back();
            i--;
        }else{
            assignations[i]->bet=false;
            assignations[i]->variable->value=1-assignations[i]->variable->value;
            //attention, si la variable venait à ne pas etre assignée
            //(value == -1), le backtrack fail lamentablement
            //ceci n'est pas possible en théorie, car toute variable dans
            //assignation est assignée
            assignations[i]->updateStatus();
            hasChanged=true;
            break;
        }
    }
    return hasChanged;
}
