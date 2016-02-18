%{
#include <iostream>
#include <cstdlib>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "expr.tab.hpp"
%}
%%
[1-9][0-9]*                                                { yylval.ival = atoi(yytext); return TK_VAR; }
^[" "]*                                                    ;
"("                                                        { return TK_LPAREN; }
")"                                                        { return TK_RPAREN; }
"/\\"                                                      { return TK_CONJ;   }
" 0"$                                                      { return TK_CONJ;   }
"\\/"                                                      { return TK_DISJ;   }
[ \t]+                                                     { return TK_DISJ;   }
"X"                                                        { return TK_XOR;    }
"=>"                                                       { return TK_IMPLY;  }
"<=>"                                                      { return TK_EQ;     }
"~"                                                        { return TK_ENOT;   }
"-"                                                        { return TK_VNOT;   }
c[^\n]*\n                                                  ;
(c[^\n]*\n)*p[ \t]+cnf[ \t]+[-]?[0-9]+[ \t]+[-]?[0-9]+[ \t]*\n    { return TK_FLINE;  }
.                                                          ;
%%
