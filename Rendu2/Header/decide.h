#ifndef DECIDE_H
#define DECIDE_H
#include <stdlib.h>
#include <time.h>
#include "formule.h"
#include "litt.h"
#include "global_variable_extern.h"


var* getFreeVar();
var* getRandFreeVar();
var* getMomsFreeVar();
var* getDlisFreeVar();

#endif // DECIDE_H
