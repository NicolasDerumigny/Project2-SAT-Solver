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
extern bool verbose;
%}
%%
[1-9][0-9]*                                                          { yylval.ival = atoi(yytext); return TK_VAR; }
^[ \t]*                                                              ;
"("                                                                  { if (verbose) cout<<"Left par\n"; return TK_LPAREN; }
")"                                                                  { if (verbose) cout<<"Right par\n"; return TK_RPAREN; }
"/\\"                                                                { if (verbose) cout<<"And par\n"; return TK_CONJ;   }
[ \t]*"0"?[ \t]*\n                                                   { if (verbose) cout<<"Conj\n"; return TK_CONJ;   }
"\\/"                                                                { if (verbose) cout<<"Disj\n"; return TK_DISJ;   }
[ \t]+                                                               { if (verbose) cout<<"Disj\n"; return TK_DISJ;   }
"X"                                                                  { if (verbose) cout<<"Xor\n"; return TK_XOR;    }
"=>"                                                                 { if (verbose) cout<<"Imply\n"; return TK_IMPLY;  }
"<=>"                                                                { if (verbose) cout<<"Equivalent\n"; return TK_EQ;     }
"~"                                                                  { if (verbose) cout<<"Negated expression\n"; return TK_ENOT;   }
"-"                                                                  { if (verbose) cout<<"Negated variable\n"; return TK_VNOT;   }
c[^\n]*\n                                                            ;
(c[^\n]*\n)*p[ \t]+cnf[ \t]+[-]?[0-9]+[ \t]+[-]?[0-9]+[ \t]*\n       ;
.                                                                    ;
%%

#ifdef __clang__
#pragma clang diagnostic pop
#endif
