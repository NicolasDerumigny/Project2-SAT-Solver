#include "cast_arg.h"


void cast_arg(int argc, char** argv){
    bool found=false;
    for (int i=1; i<argc;i++){
        if (!strcmp(argv[i],"-tseitin"))
            isTseitin=true;

        if (!strcmp(argv[i],"-v"))
            verbose=true;
		if (!strcmp(argv[i],"-vv")){
			verbose2=true;
			verbose=true;
		}

        if (!strcmp(argv[i],"-vvp")){
            verbose2=true;
            verbose=true;
            verboseParser=true;
        }


        if (!strcmp(argv[i],"-vp")){
            verboseParser=true;
        }

        if (!strcmp(argv[i],"-time"))
            timePerf=true;

        if (!strcmp(argv[i],"-rand"))
                heuristic=1;
        if (!strcmp(argv[i],"-moms"))
                heuristic=2;
        if (!strcmp(argv[i],"-dlis"))
                heuristic=3;
        if (!strcmp(argv[i],"-vsids"))
                heuristic=4;

        if (!strcmp(argv[i],"-cl"))
            clLearning=true;

        if (!strcmp(argv[i],"-cl-interac")){
            interactive=true;
            clLearning=true;
        }

        if (!strcmp(argv[i],"-wl"))
            wl=true;

        if (!strcmp(argv[i],"-explainunsat"))
            proof=true;

        if (strlen(argv[i])<1024 and !found){
                     char buf[1024]="";
                     strcpy(buf,argv[i]);
                     std::reverse(buf, &buf[strlen(buf)]);
                     if(!strncmp(buf,"fnc.",4)){
                         strcpy(path,argv[i]);
                         found=true;
                     }
                     if(!strncmp(buf,"rof.",4)){
                         strcpy(path,argv[i]);
                         found=true;
                     }
        }



    }
}
