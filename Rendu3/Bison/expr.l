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
extern bool verboseParser;
%}
%%
[1-9][0-9]*                                                                      { yylval.ival = atoi(yytext); return TK_VAR; }
^[ \t]*                                                                          ;
[ \t]*"("[ \t]*                                                                  { if (verboseParser) cerr<<"Left par\n"; return TK_LPAREN; }
[ \t]*")"[ \t]*                                                                  { if (verboseParser) cerr<<"Right par\n"; return TK_RPAREN; }
[ \t]*"/\\"[ \t]*                                                                { if (verboseParser) cerr<<"And\n"; return TK_CONJ;   }
[ \t]*"0"?[ \t]*\n                                                               { if (verboseParser) cerr<<"Conj\n"; return TK_CONJ;   }
[ \t]*"\\/"[ \t]*                                                                { if (verboseParser) cerr<<"Disj\n"; return TK_DISJ;   }
[ \t]*"X"[ \t]*                                                                  { if (verboseParser) cerr<<"Xor\n"; return TK_XOR;    }
[ \t]*"=>"[ \t]*                                                                 { if (verboseParser) cerr<<"Imply\n"; return TK_IMPLY;  }
[ \t]*"<=>"[ \t]*                                                                { if (verboseParser) cerr<<"Equivalent\n"; return TK_EQ;     }
[ \t]+                                                                           { if (verboseParser) cerr<<"Disj\n"; return TK_DISJ;   }
[ \t]*"~"[ \t]*                                                                  { if (verboseParser) cerr<<"Negated expression\n"; return TK_ENOT;   }
"-"                                                                              { if (verboseParser) cerr<<"Negated variable\n"; return TK_VNOT;   }
c[^\n]*\n                                                                        ;
(c[^\n]*\n)*p[ \t]+cnf[ \t]+[-]?[0-9]+[ \t]+[-]?[0-9]+[ \t]*\n                   ;
.                                                                                ;
%%

#ifdef __clang__
#pragma clang diagnostic pop
#endif
