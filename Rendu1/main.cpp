#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;


#include "expr.hpp"
#include "expr.tab.hpp"
#include "assignation.cpp"
vector<assignation*> assignations;
//création du vector qui contient les assignations réalisées

#include "var.cpp"
vector<var*> v_var;
//création du vector qui contiendra tous les pointeurs vers les variables



#include "litt.cpp"
#include "clause.cpp"
#include "formule.cpp"
formule * instance;
//une formule pour les gouverner tous

#include "decide.cpp"
#include "deduce.cpp"
#include "backtrack.cpp"

//extern "C" int yyparse();
extern "C" FILE *yyin;
extern Expr *res;
#include "file_open.cpp"

int main(int argc, char** argv) {
    checkCorrectFile(argv[1]);
    checkRightArg(argc, argv[0]);
    checkHeader(argv[1]);
    parse(argv[1]);

    while(getFreeVar()!=nullptr){
        //decide
        getFreeVar()->assignValue(1,true);
        //on fait un paris : la freeVar est à vrai

        //deduce


        //backtrack
        while(!check()){
            if(!backtrack()){
                cout<<"s UNSATISFIABLE"<<endl;
                freeAll();
                return 0;
            }
        }
        //verifier que tout est cohérent
    }


    cout<<"s SATISFIABLE"<<endl;


    freeAll();
    return 0;
}

