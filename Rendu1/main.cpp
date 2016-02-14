#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#include "expr.hpp"
#include "expr.tab.hpp"
#include "var.cpp"
vector<var*> v_var;//création du vector qui contiendra tous les pointeurs vers les variables

#include "litt.cpp"
#include "clause.cpp"
#include "formule.cpp"


//extern "C" int yyparse();
extern "C" FILE *yyin;
extern Expr *res;

int main(int argc, char** argv) {
    if (argc==1){

        struct stat buff;
        if (lstat(argv[1], &buff)){
            if (!S_ISREG(buff.st_mode)){
                cout<<"This file specified is not a regular file"<<endl;
                return 1;
            }//on vérifie que le fichier en entrée est bien un fichier normal
        }else{
            cout<<"Failed to open file"<<endl;
            return 1;
        }

        {
            ofstream eFile;//EntryFile
            eFile.open(argv[1], ofstream::out | ofstream::app);
            eFile<<'x';
            eFile.close();//on rajout un caractère pour faciliter la lecture syntaxique
        }

        ifstream eFile;
        eFile.open(argv[1], ifstream::in);

        string p,cnf,lastLine;
        int V, C;

        eFile >> p >> cnf >> V >> C;
        //A FAIRE : garder la derniere ligne;
        eFile.close();



        if (p!="p" or cnf!="cnf"){
            cout<<"Bad file format : header should be :"<< endl;
            cout<<"p cnf V C"<<endl;
            cout<<"Where V is the maximum number of variables, and C the number of clauses"<<endl;
            return 1;
        }

        for(int i=0;i<V;i++)
            v_var.push_back(nullptr);


        FILE* inputFile;
        inputFile=fopen(argv[1],"r");

        // parse through the input until there is no more:
        yyin = inputFile;
        do {
            yyparse();
            cout << res->to_string() << endl ;//<< res->eval() << endl;
        } while (!feof(yyin));

        fclose(inputFile);
        {
            ofstream eFile;//EntryFile
            eFile.open(argv[1], ofstream::out | ofstream::app);
            //A FAIRE : remettre la derniere ligne;
            eFile.close();
        }


        return 0;

    }else{
        cout<<"wrong number of parameters !"<<endl;
        cout<<"Usage : "<<argv[0]<<" fichier.cnf\n";
        return 1;
    }
}

void yyerror(const char *s) {
    cout << "EEK, parse error!  Message: " << s << endl;
    // might as well halt now:
    exit(-1);
}

