#include "../Header/cast_arg.h"

void cast_arg(int argc, char** argv){
    for (int i=1; i<argc;i++){
        if (!strcmp(argv[i],"-tseitin"))
            isTseitin=true;

        if (!strcmp(argv[i],"-v"))
            verbose=true;

        if (!strcmp(argv[i],"-time"))
            timePerf=true;

    }
}
