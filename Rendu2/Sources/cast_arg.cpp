#include <string.h>

bool is_tseitin(int argc, char** argv){
	for (int i=1; i<argc;i++)
        if (strncmp(argv[i],"-tseitin",8) and (strlen(argv[i])==8) )
            return true;
    return false;
}
