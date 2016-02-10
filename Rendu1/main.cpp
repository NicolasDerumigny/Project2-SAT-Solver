#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
using namespace std;

#include "expr.hpp"
#include "expr.tab.hpp"
#include "clause.cpp"
#include "var.cpp"
#include "litt.cpp"
#include "formule.cpp"

//extern "C" int yyparse();
extern "C" FILE *yyin;
extern Expr *res;



int main(int, char**) {
    // parse through the input until there is no more:
    do {
        yyparse();
        cout << res->to_string() << endl << res->eval() << endl;
    } while (!feof(yyin));
    return 0;
}

void yyerror(const char *s) {
    cout << "EEK, parse error!  Message: " << s << endl;
    // might as well halt now:
    exit(-1);
}
