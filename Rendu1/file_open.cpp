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
    eFile.open(file_dir, ifstream::in);



    string p,cnf;
    int V, C=-1, D=-1;

    int nbr_line=1,s=0;
    string line;
    while(getline(eFile, line)){
        nbr_line++;
        if (line[0]!='c'){
            s=line.size();
            break;
        }
    }
    {
        stringstream str;
        str << line;
        str >> p >> cnf >> V >> C >> D;
        str.str("");
    }

















    if((7+1*(        V<=(1        *01       -1))+(C<=0)//+
    +//              ||  \\\      $@4       c=|)
    ceil(log(        1*   abs     (V)        +000+1)/log\
    (10              /*    */)    )+1               *ceil
    (log(abs(        C)    +1)/log(10       ))-s))goto l;

            /*~~~~~~(: syntax nazi :)~~~~~~*/



















    if (p!="p" or cnf!="cnf" or C==-1 or D!=-1){l:
        cout<<"Bad file format: header should be before clause declaration and respect scrupulously the following syntax:"<< endl;
        cout<<"p cnf V C"<<endl;
        cout<<"Where V is the maximum number of variables, and C the number of clauses"<<endl;
        if (p!="p" or cnf!="cnf" or C==-1 or D!=-1)
            exit(-1);
    }


    int nbr_C=0;
    while(getline(eFile, line)){
        if (line[0]!='c')
            nbr_C++;
        int size=line.size();

        {
            stringstream str;
            reverse(line.begin(), line.end());
            str << line;
            str >> D;
        }

        if (line[0]!='0'){
            cout<<"Warning (line "<< nbr_line<<"): Non-comments lines must end by 0 and ends by one or more spaces"<<endl;
        }
        if (line[size-1]!='c' and D!=0){
            //si ce n'est pas un commentaire et que la string ne se termine pas par " 0"
            cout<<"Error (line "<< nbr_line<<"): Non-comments lines must end by 0, exiting "<<endl;
            exit(-1);
        }
        /*if (line!="c xxx" and line[0]=='c'){
            cout<<"Warning (line "<< nbr_line<<"): comment line is not scrupulously \"c xxx\", continuing anyway."<<endl;
        }*/
        if (line[size-1]=='c' and line[size-2]==' '){
            cout<<"Warning (line "<< nbr_line<<"): comments lines must be scrupulously of the form \"c xxx\", continuing anyway."<<endl;
        }
        nbr_line++;
    }


    if (nbr_C!=C)
        cout<<"Warning : "<<C<<" clauses were declared in header line but found "<< nbr_C <<", continuing anyway"<<endl;
    eFile.close();

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
            cout << "Formula in input:"<<endl;
            cout <<res->to_string() << endl;
            instance =  res->eval();
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
    for (unsigned long i=0; i<assignations.size(); i++){
        delete assignations[i];
        assignations[i]=nullptr;
    }
}

void print_output(){
    for(int i=1; i<int(v_var.size());i++){
        if (v_var[i]!=nullptr)
            v_var[i]->print();
        else
            cout<<i;
        cout<<" ";
    }
    cout<<0<<endl;
}
