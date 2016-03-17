#include <string.h>

void cast_arg(int argc, char** argv){
    for (int i=1; i<argc;i++){
        if (strncmp(argv[i],"-tseitin",8) and (strlen(argv[i])==8) )
            isTseitin=true;

        if (strncmp(argv[i],"-v",8) and (strlen(argv[i])==8) )
            verbose=true;

        if (strncmp(argv[i],"-time",8) and (strlen(argv[i])==8) )
            timePerf=true;

    }
}
