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
#include "prooftree.h"


int main(int argc, char** argv) {
    cast_arg(argc, argv);
    if (timePerf) fprintf(stderr,"begin: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);

    if(!isTseitin){
        checkCorrectFile();
        checkHeaderAndParse();
    }
    if (timePerf){
        checkpoint = clock();
        fprintf(stderr,"check: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    }

    if(proof){
        setNewProofTree();
    }

    parse();

    if(verbose) instance->print();

	instance->preprocessing();
	if(verbose) {cout << "Formula after preprocessing :\n";instance->print();}

	var* new_var = nullptr;
	if (heuristic == 0)
		new_var = getFreeVar();
	else if (heuristic == 1)
		new_var = getRandFreeVar();
	else if (heuristic == 2)
		new_var = getMomsFreeVar();
	else if (heuristic == 3)
		new_var = getDlisFreeVar();

    clause* cl_conflict = nullptr;

    while(new_var!=nullptr){
		//decide (suite)
		new_var->assignValue(1,true);
        //on fait un pari : la freeVar de decide est à vrai
		
		if(verbose2) {
            cout<<"\n------------Next step after decide:-----------------\n";
            instance->print();
        }
		if (verbose){
			fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
			for (auto& ass:assignations)
				fprintf(stderr,"[%i,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
			fprintf(stderr,"\n\n");
		}
		
        //deduce
        while(assignUniqueLitt() || ((!clLearning) && (assignUniquePolarity()) ) ){
            if(verbose2) {
	            cout<<"\n------------Next step after deduce:-----------------\n";
	            instance->print();
	        }
			if (verbose){
				fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
				for (auto& ass:assignations)
					fprintf(stderr,"[%i,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
				fprintf(stderr,"\n\n");
			}
			continue;
		}

        if(verbose2) {
            cout<<"\n------------Next step after deduce:-----------------\n";
            instance->print();
        }
		if (verbose){
			fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
			for (auto& ass:assignations)
				fprintf(stderr,"[%i,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
			fprintf(stderr,"\n\n");
		}

        //backtrack
        cl_conflict = getConflict();
        while(cl_conflict != nullptr){
            if( ((clLearning) && (!conflictAnal(cl_conflict))) || ((!clLearning) && (!backtrack())) ){
                if (verbose){
					fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
					for (auto& ass:assignations)
						fprintf(stderr,"[%i,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
					fprintf(stderr,"\n\n");
				}
				cout<<"s UNSATISFIABLE"<<endl;
                freeAll();
                if (timePerf){
                    checkpoint = clock();
                    fprintf(stderr,"end: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
                }
                return 0;
            }
            cl_conflict=getConflict();
        }
		
		if(verbose2) {
            cout<<"\n------------Next step after backtrack:-----------------\n";
            instance->print();
        }
		if (verbose){
			fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
			for (auto& ass:assignations)
				fprintf(stderr,"[%i,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
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

