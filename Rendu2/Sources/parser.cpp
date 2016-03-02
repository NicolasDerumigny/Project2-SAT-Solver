bool parse_manual(string line, int nbLine){
    int i=0;
    int len=line.size();
    while ((line[i]==' ' or line[i]=='\t') and i<len)
        i++;
    if (i==len){
        cerr<<"Warning : entry contain an empty line (line "<< nbLine <<" )"<<endl;
    }
    if (line[i]=='c'){
       return true;
    }
    return true;
}
