#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
#include "../Header/global_variables.h"


#include "../Header/var.h"
#include "../Header/litt.h"
#include "../Header/clause.h"
#include "../Header/formule.h"
#include "../Header/assignation.h"

#include "../Header/decide.h"
#include "../Header/deduce.h"
#include "../Header/backtrack.h"

#include "../Header/file_open.h"
#include "../Header/cast_arg.h"


int main(int argc, char** argv) {
    cast_arg(argc, argv);
    if (timePerf) fprintf(stderr,"begin: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);

    if(!isTseitin){
        checkCorrectFile(argv[1]);
        checkHeaderAndParse(argv[1]);
    }
    if (timePerf){
        checkpoint = clock();
        fprintf(stderr,"check: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    }
    parse(argv[1]);

    if(verbose) instance->print();

    while(getDlisFreeVar()!=nullptr){
        //decide
        getDlisFreeVar()->assignValue(1,true);
        //on fait un pari : la freeVar est à vrai

        //deduce
		while(assignUniqueLitt() or assignUniquePolarity())
            continue;

        if(verbose) instance->print();

        //backtrack
        while(!check()){
            if(!backtrack()){
                cout<<"s UNSATISFIABLE"<<endl;
                freeAll();
                return 0;
            }
        }
    }
    cout<<"s SATISFIABLE"<<endl;
    print_output();
    freeAll();
    if (timePerf){
        checkpoint = clock();
        fprintf(stderr,"end: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    }

    return 0;
}

