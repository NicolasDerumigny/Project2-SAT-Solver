#ifndef FILE_OPEN_H
#define FILE_OPEN_H
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include "assignation.h"
#include <algorithm>
#include "../Bison/expr.hpp"
#include "../Bison/expr.tab.hpp"
#include "global_variable_extern.h"
#include "prooftree.h"
using namespace std;
extern "C" FILE *yyin;
extern Expr *res;
extern int yylex_destroy  (void);

void checkCorrectFile();
void checkHeaderAndParse();
void yyerror(const char *s);
void parse();
void freeAll();
void print_output();


#endif // FILE_OPEN_H
