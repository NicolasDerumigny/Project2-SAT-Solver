#include <vector>

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
    if(verbose) {std::cout << "Formula after preprocessing :\n";instance->print();}

	var* new_var = nullptr;

    clause* cl_conflict = nullptr;

    while(instance->f_ClauseUnsatisfied!=nullptr && (satModEq && checkAssign())){
		
        //deduce
        while(assignUniqueLitt() || ((!clLearning) && (assignUniquePolarity()) ) ){//si clLearning alors on ne lance pas assignUniquePolarity car && est paresseux
            if(verbose2) {
                std::cout<<"\n------------Next step after deduce:-----------------\n";
	            instance->print();
	        }
			if (verbose){
				fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
				for (auto& ass:assignations)
                    fprintf(stderr,"[%lu,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
				fprintf(stderr,"\n\n");
			}
			continue;
		}

        if(verbose2) {
            std::cout<<"\n------------Next step after deduce:-----------------\n";
            instance->print();
        }
		if (verbose){
			fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
			for (auto& ass:assignations)
                fprintf(stderr,"[%lu,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
			fprintf(stderr,"\n\n");
		}

        //vérifie si conflit
        cl_conflict = getConflict();
        if (cl_conflict != nullptr){//Conflit détecté, backtrack ou analyse de conflit
            if( ((clLearning) && (!conflictAnal(cl_conflict))) || ((!clLearning) && (!backtrack())) ){
                if (verbose){
                    fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
                    for (auto& ass:assignations)
                        fprintf(stderr,"[%lu,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
                    fprintf(stderr,"\n\n");
                }
                std::cout<<"s UNSATISFIABLE"<<std::endl;
                freeAll();
                if (timePerf){
                    checkpoint = clock();
                    fprintf(stderr,"end: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
                }
                return 0;
            }

            if(verbose2) {
                std::cout<<"\n------------Next step after backtrack:-----------------\n";
                instance->print();
            }
            if (verbose){
                fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
                for (auto& ass:assignations)
                    fprintf(stderr,"[%lu,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
                fprintf(stderr,"\n\n");
            }
        } else {//Pas de conflit, decide
            if (heuristic == 0)
                new_var = getFreeVar();
            else if (heuristic == 1)
                new_var = getRandFreeVar();
            else if (heuristic == 2)
                new_var = getMomsFreeVar();
            else if (heuristic == 3)
                new_var = getDlisFreeVar();
            else if (heuristic == 4)
                new_var = getVsidsVar();
            if (new_var != nullptr) {
                if (clLearning)
                    level_cur++;
                new_var->assignValue(1,true);//on fait un pari : la freeVar de decide est à vrai
            } else //On a plus de variables libres et pas de conflit
                break;

            if(verbose2) {
                std::cout<<"\n------------Next step after decide:-----------------\n";
                instance->print();
            }
            if (verbose){
                fprintf(stderr,"heap of assignations : [var_id,bet,value]\n");
                for (auto& ass:assignations)
                    fprintf(stderr,"[%lu,%i,%i],",ass->variable->id,ass->bet,ass->variable->value);
                fprintf(stderr,"\n\n");
            }
        }
    }
    std::cout<<"s SATISFIABLE"<<std::endl;
    print_output();
    freeAll();
    if (timePerf){
        checkpoint = clock();
        fprintf(stderr,"end: %f s\n",(double) checkpoint/CLOCKS_PER_SEC);
    }

    return 0;
}
