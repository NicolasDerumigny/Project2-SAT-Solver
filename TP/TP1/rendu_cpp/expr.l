%{
#include <iostream>
#include <cstdlib>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "expr.tab.hpp"
%}
%%
[ \t\n]         ;
[1-9]+[0-9]*          { yylval.ival = atoi(yytext); return TK_VAR; }
"("             { return TK_LPAREN; }
")"             { return TK_RPAREN; }
"/\\"            { return TK_CONJ; }
"\n"            { return TK_CONJ; }
"\\/"            { return TK_DISJ; }
"X"             { return TK_XOR; }
"=>"            { return TK_IMPLY; }
"<=>"           { return TK_EQ; }
"~"             { return TK_ENOT; }
"0"             { return TK_EOF; }
"-"             { return TK_VNOT; }
.               ;
%%
