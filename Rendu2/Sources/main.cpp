#include <vector>
using namespace std;
#include "global_variables.h"


#include "var.h"
#include "litt.h"
#include "clause.h"
#include "formule.h"
#include "assignation.h"

#include "decide.h"
#include "deduce.h"
#include "backtrack.h"

#include "file_open.h"
#include "cast_arg.h"


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

//	instance->preprocessing();
//	if(verbose) {cout << "Formula after preprocessing :\n";instance->print();}
	var* new_var = nullptr;
	if (heuristic == 0)
		new_var = getFreeVar();
	else if (heuristic == 1)
		new_var = getRandFreeVar();
	else if (heuristic == 2)
		new_var = getMomsFreeVar();
	else if (heuristic == 3)
		new_var = getDlisFreeVar();

    while(new_var!=nullptr){
		//decide (suite)
		new_var->assignValue(1,true);
        //on fait un pari : la freeVar de decide est à vrai
		
        //deduce
		while(assignUniqueLitt() or assignUniquePolarity())
            continue;

        if(verbose) {
            cout<<"\n------------Next step :-----------------\n";
            instance->print();
        }

        //backtrack
        while(!check()){
            if(!backtrack()){
                cout<<"s UNSATISFIABLE"<<endl;
                freeAll();
                return 0;
            }
        }
		
		if (verbose){
			fprintf(stderr,"heap of assignations : [var_id,bet]");
			for (auto& ass:assignations)
				fprintf(stderr,"[%i,%i],",ass->variable->id,ass->bet);
			fprintf(stderr,"\n\n");
		}
		
		//decide
        if (heuristic == 0)
			new_var = getFreeVar();
		else if (heuristic == 1)
			new_var = getRandFreeVar();
		else if (heuristic == 2)
			new_var = getMomsFreeVar();
		else if (heuristic == 3)
			new_var = getDlisFreeVar();
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

