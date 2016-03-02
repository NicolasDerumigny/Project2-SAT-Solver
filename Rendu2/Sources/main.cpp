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
#include "global_variables.h"
#include "../Bison/expr.hpp"
#include "../Bison/expr.tab.hpp"

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
    checkCorrectFile(argv[1]);
    checkRightArg(argc, argv[0]);
    checkHeader(argv[1]);
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
    return 0;
}
