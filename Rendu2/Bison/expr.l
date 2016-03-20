%{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"
#endif

#pragma GCC diagnostic ignored "-Wunused-function"

#include <iostream>
#include <cstdlib>
using namespace std;
#define YY_DECL extern "C" int yylex()
#include "expr.tab.hpp"
%}
%%
[1-9][0-9]*                                                          { yylval.ival = atoi(yytext); return TK_VAR; }
^[ \t]*                                                              ;
"("                                                                  { return TK_LPAREN; }
")"                                                                  { return TK_RPAREN; }
"/\\"                                                                { cerr<<"Conjunction"<<endl; return TK_CONJ;   }
[ \t]+"0"[ \t]*\n                                                    { cerr<<"Conjunction"<<endl; return TK_CONJ;   }
"\\/"                                                                { cerr<<"Disjunction"<<endl; return TK_DISJ;   }
[ \t]+                                                               { cerr<<"Disjunction"<<endl; return TK_DISJ;   }
"X"                                                                  { return TK_XOR;    }
"=>"                                                                 { return TK_IMPLY;  }
"<=>"                                                                { return TK_EQ;     }
"~"                                                                  { cerr<<"Expression negated"<<endl; return TK_ENOT;   }
"-"                                                                  { cerr<<"Var negated"<<endl; return TK_VNOT;   }
c[^\n]*\n                                                            ;
((c[^\n]*\n)*p[ \t]+cnf[ \t]+[-]?[0-9]+[ \t]+[-]?[0-9]+[ \t]*\n)?    { return TK_FLINE;  }
.                                                                    ;
%%

#ifdef __clang__
#pragma clang diagnostic pop
#endif
