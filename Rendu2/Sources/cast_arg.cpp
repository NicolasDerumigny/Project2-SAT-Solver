bool is_tseitin(int argc, char** argv){
    for (int i=1; i<=argc;i++)
        if (argv[i]=="-tseitin")
            return true;
    return false
}
