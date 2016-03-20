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
#include <ctime>
using namespace std;
clock_t checkpoint = clock();
#include "global_variables.h"
#include "expr.hpp"
#include "expr.tab.hpp"

#include "var.cpp"
#include "litt.cpp"
#include "clause.cpp"
#include "formule.cpp"
#include "assignation.cpp"

#include "decide.cpp"
#include "deduce.cpp"
#include "backtrack.cpp"
#include "file_open.cpp"

int main(int argc, char** argv) {
    fprintf(stderr,"begin: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    checkCorrectFile(argv[1]);
    checkRightArg(argc, argv[0]);
    checkHeader(argv[1]);
    checkpoint = clock();
    fprintf(stderr,"check: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);

    parse(argv[1]);

    while(getFreeVar()!=nullptr){
        //decide
        getFreeVar()->assignValue(1,true);
        //on fait un pari : la freeVar est à vrai

        //deduce
		while(assignUniqueLitt() or assignUniquePolarity())
            continue;

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
    checkpoint = clock();
    fprintf(stderr,"end: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    return 0;
}
