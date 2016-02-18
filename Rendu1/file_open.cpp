//extern "C" int yyparse();
extern "C" FILE *yyin;
extern Expr *res;

void checkCorrectFile(char* file_dir){
    struct stat buff;
    if (!stat(file_dir, &buff)){//si l'ouverture à fonctionnée
        if (!S_ISREG(buff.st_mode)){
            cout<<"This file specified is not a regular file"<<endl;
            exit(-1);
        }//on vérifie que le fichier en entrée est bien un fichier normal
    }else{
        cout<<"Failed to open file : "<<file_dir<<endl;
        exit(-1);
    }
}

void checkRightArg(int nbrOfArg, char* exeFile){
    if (nbrOfArg!=2){
            cout<<"Wrong number of parameters !"<<endl;
            cout<<"Usage : "<<exeFile<<" fichier.cnf\n";
            exit(-1);
    }
}

void checkHeader(char* file_dir){
    ifstream eFile;
    string temp;
    eFile.open(file_dir, ifstream::in);

    string p,cnf;
    int V, C;

    eFile >> p >> cnf >> V >> C;
    eFile.close();


    if (p!="p" or cnf!="cnf"){
        cout<<"Bad file format : header should be :"<< endl;
        cout<<"p cnf V C"<<endl;
        cout<<"Where V is the maximum number of variables, and C the number of clauses"<<endl;
        exit(-1);
    }

    for(int i=0;i<=V;i++)
        v_var.push_back(nullptr);
}

void yyerror(const char *s) {
    cout << "EEK, parse error!  Message: " << s << endl;
    // might as well halt now:
    exit(-1);
}

void parse(char* file_dir){
    FILE* inputFile;
    inputFile=fopen(file_dir,"r");
    if(inputFile!=nullptr){
        // parse through the input until there is no more:
        yyin = inputFile;
        do {
            yyparse();
            cout << res->to_string() << endl;
            instance =  res->eval();
            instance->print();
        } while (!feof(yyin));

        fclose(inputFile);
    }else{
        cout<<"Error : couldn't open file : "<<file_dir<<endl;
        exit(-1);
    }
}

void freeAll(){
    instance->free_formule();
    for (unsigned long i=0; i<v_var.size(); i++)
        delete v_var[i];
}

void print_output(){
    for(int i=1; i<int(v_var.size());i++){
        v_var[i]->print();
        cout<<" ";
    }
    cout<<0<<endl;
}
